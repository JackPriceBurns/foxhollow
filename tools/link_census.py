#!/usr/bin/env python3
import os
import re
import subprocess
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

PREFIXES = [
    ("OS", "os"), ("DVD", "dvd"), ("GX", "gx"), ("__GX", "gx"), ("VI", "vi"),
    ("PAD", "pad"), ("CARD", "card"), ("AR", "aram"), ("ARQ", "aram"),
    ("AI", "ai"), ("AX", "ax"), ("DSP", "dsp"), ("EXI", "exi"), ("SI", "si"),
    ("GBA", "gba"), ("THP", "thp"), ("MTX", "mtx"), ("PSMTX", "mtx"),
    ("C_MTX", "mtx"), ("DB", "db"), ("L2", "cache"), ("LC", "cache"),
    ("DC", "cache"), ("IC", "cache"), ("PPC", "ppc"),
]


def classify(sym):
    for prefix, bucket in sorted(PREFIXES, key=lambda p: -len(p[0])):
        if sym.startswith(prefix):
            return bucket
    if sym.startswith("__"):
        return "runtime"
    return "game"


def main():
    proc = subprocess.run(
        ["cmake", "--build", os.path.join(ROOT, "build"), "--target", "linkcheck"],
        capture_output=True,
        text=True,
        errors="replace",
    )
    out = proc.stdout + proc.stderr
    if proc.returncode == 0:
        print("linkcheck LINKS CLEAN — no undefined symbols")
        return 0

    syms = sorted(set(re.findall(r'"_(\w+)", referenced from', out)))
    if not syms:
        print("link failed but no undefined-symbol lines parsed; raw tail:")
        print("\n".join(out.splitlines()[-30:]))
        return 1

    buckets = defaultdict(list)
    for s in syms:
        buckets[classify(s)].append(s)

    print(f"{len(syms)} undefined symbols\n")
    for bucket in sorted(buckets, key=lambda b: -len(buckets[b])):
        names = buckets[bucket]
        print(f"{bucket:10s} {len(names)}")
        for n in names:
            print(f"    {n}")

    with open(os.path.join(ROOT, "port", "undefined.txt"), "w") as f:
        for bucket in sorted(buckets):
            for n in buckets[bucket]:
                f.write(f"{bucket} {n}\n")
    print(f"\nwrote port/undefined.txt")
    return 1


if __name__ == "__main__":
    sys.exit(main())
