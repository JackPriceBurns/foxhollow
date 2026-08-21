#!/usr/bin/env python3
"""Extract one release section from CHANGELOG.md."""

import argparse
import json
import re
import sys

HEADING = re.compile(r"^##\s+(.+?)\s*$")


def section_for(text, version):
    lines = text.splitlines()
    wanted = version.strip()
    body = None

    for index, line in enumerate(lines):
        match = HEADING.match(line)
        if match is None:
            continue
        title = match.group(1)
        name = re.split(r"\s+[-–—]\s+", title, maxsplit=1)[0].strip()
        name = name.strip("[]")
        if name != wanted:
            continue
        body = []
        for following in lines[index + 1 :]:
            if HEADING.match(following):
                break
            body.append(following)
        break

    if body is None:
        return None
    return "\n".join(body).strip("\n")


def bullets(body):
    items = []
    for line in body.splitlines():
        stripped = line.strip()
        if stripped.startswith(("- ", "* ")):
            items.append(stripped[2:].strip())
        elif items and stripped and not stripped.startswith("#"):
            items[-1] = f"{items[-1]} {stripped}"
    return items


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", required=True)
    parser.add_argument("--file", default="CHANGELOG.md")
    parser.add_argument("--format", choices=["markdown", "json"], default="markdown")
    args = parser.parse_args()

    with open(args.file, encoding="utf-8") as handle:
        body = section_for(handle.read(), args.version)

    if body is None:
        print(f"No section for {args.version} in {args.file}", file=sys.stderr)
        return 1

    if args.format == "json":
        print(json.dumps(bullets(body)))
    else:
        print(body)
    return 0


if __name__ == "__main__":
    sys.exit(main())
