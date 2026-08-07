#!/usr/bin/env python3
import argparse
import concurrent.futures
import os
import re
import subprocess
import sys
from collections import Counter

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GAME_SRC = os.path.join(ROOT, "game", "src")
LIST_PATH = os.path.join(ROOT, "port", "compiling.txt")

FLAGS = [
    "-std=gnu11",
    "-fsyntax-only",
    "-fsigned-char",
    "-w",
    "-DBUILD_VERSION=0",
    "-DVERSION_GSAE01",
    "-DNDEBUG=1",
    "-DTARGET_PC=1",
    "-include",
    os.path.join(ROOT, "port", "include", "foxhollow_compat.h"),
    "-I" + os.path.join(ROOT, "port", "include"),
    "-I" + os.path.join(ROOT, "extern", "aurora", "include"),
    "-I" + os.path.join(ROOT, "game", "include"),
    "-I" + os.path.join(ROOT, "game"),
]


def all_sources():
    out = []
    for dirpath, _, files in os.walk(GAME_SRC):
        for f in sorted(files):
            if f.endswith(".c"):
                out.append(os.path.relpath(os.path.join(dirpath, f), GAME_SRC))
    return sorted(out)


def try_compile(rel):
    proc = subprocess.run(
        ["clang"] + FLAGS + [os.path.join(GAME_SRC, rel)],
        capture_output=True,
        text=True,
        errors="replace",
    )
    return rel, proc.returncode == 0, proc.stderr


def first_error(stderr):
    for line in stderr.splitlines():
        m = re.match(r".*error: (.*)", line)
        if m:
            return m.group(1)
    return "(no error line)"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--update", action="store_true", help="rewrite port/compiling.txt with passing TUs")
    ap.add_argument("--errors", type=int, default=0, metavar="N", help="show top N error signatures")
    ap.add_argument("--fails", type=int, default=0, metavar="N", help="list first N failing files with their first error")
    ap.add_argument("--only", help="restrict to files whose path contains this substring")
    args = ap.parse_args()

    sources = all_sources()
    if args.only:
        sources = [s for s in sources if args.only in s]

    passed, failed = [], []
    with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count()) as ex:
        for rel, ok, stderr in ex.map(try_compile, sources):
            (passed if ok else failed).append((rel, stderr))

    print(f"{len(passed)}/{len(sources)} TUs compile ({100.0 * len(passed) / max(len(sources), 1):.1f}%)")

    if args.errors:
        counts = Counter(first_error(stderr) for _, stderr in failed)
        for msg, n in counts.most_common(args.errors):
            print(f"{n:5d}  {msg}")

    if args.fails:
        for rel, stderr in failed[: args.fails]:
            print(f"FAIL {rel}: {first_error(stderr)}")

    if args.update and not args.only:
        with open(LIST_PATH, "w") as f:
            f.write("# TUs that compile under clang; maintained by tools/compile_progress.py --update\n")
            for rel, _ in sorted(passed):
                f.write(rel + "\n")
        print(f"wrote {LIST_PATH}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
