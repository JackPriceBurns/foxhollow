#!/usr/bin/env python3
"""Create a release through the API and upload each built binary to its presigned URL."""

import json
import os
import pathlib
import subprocess
import sys

import requests
from botocore.auth import SigV4Auth
from botocore.awsrequest import AWSRequest
from botocore.session import Session

API_HOST = os.environ.get("API_HOST", "api.foxhollow.dev")
REGION = os.environ.get("AWS_REGION", "eu-west-1")
CHANNEL = os.environ.get("RELEASE_CHANNEL", "stable")
DIST = pathlib.Path("dist")

EXTENSIONS = {"macos": "tar.gz", "linux": "tar.gz", "windows": "zip"}


def notes_for(tag):
    result = subprocess.run(
        [sys.executable, ".github/scripts/changelog.py", "--version", tag, "--format", "json"],
        capture_output=True,
        text=True,
        check=True,
    )
    return json.loads(result.stdout)


def signed_post(url, payload):
    body = json.dumps(payload)
    request = AWSRequest(
        method="POST",
        url=url,
        data=body,
        headers={"content-type": "application/json"},
    )
    credentials = Session().get_credentials()
    if credentials is None:
        raise SystemExit("No AWS credentials available to sign the request.")
    SigV4Auth(credentials.get_frozen_credentials(), "execute-api", REGION).add_auth(request)

    response = requests.post(url, data=body, headers=dict(request.headers), timeout=30)
    if response.status_code >= 300:
        raise SystemExit(f"Create release failed ({response.status_code}): {response.text}")
    return response.json()


def built_artifacts(tag):
    found = {}
    for platform, extension in EXTENSIONS.items():
        local = DIST / f"foxhollow-{tag}-{platform}.{extension}"
        if local.exists():
            found[platform] = local
    return found


def main():
    tag = os.environ["TAG"]
    artifacts = built_artifacts(tag)
    payload = {
        "version": tag,
        "channel": CHANNEL,
        "commit": os.environ.get("COMMIT", "")[:12],
        "notes": notes_for(tag),
        "sizeBytes": max((path.stat().st_size for path in artifacts.values()), default=0),
    }
    release_url = os.environ.get("RELEASE_URL")
    if release_url:
        payload["releaseUrl"] = release_url

    release = signed_post(f"https://{API_HOST}/releases", payload)
    print(f"Created {release['version']} on the {release['channel']} channel.")

    uploaded = 0
    for upload in release["uploads"]:
        platform = upload["platform"]
        local = artifacts.get(platform)
        if local is None:
            print(f"  {platform}: not built, skipping")
            continue
        with local.open("rb") as handle:
            response = requests.put(upload["uploadUrl"], data=handle, timeout=900)
        if response.status_code >= 300:
            raise SystemExit(f"  {platform}: upload failed ({response.status_code})")
        print(f"  {platform}: uploaded to {upload['downloadUrl']}")
        uploaded += 1

    if uploaded == 0:
        raise SystemExit("No binaries were uploaded.")


if __name__ == "__main__":
    main()
