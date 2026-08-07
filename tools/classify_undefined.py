#!/usr/bin/env python3
import os
import re
import subprocess
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SFA = os.path.expanduser("~/Code/sfa")


def load_undefined():
    syms = []
    with open(os.path.join(ROOT, "port", "undefined.txt")) as f:
        for line in f:
            bucket, name = line.split()
            if bucket == "game":
                syms.append(name)
    return syms


def load_symbols_txt():
    info = {}
    path = os.path.join(SFA, "config", "GSAE01", "symbols.txt")
    pat = re.compile(r"^(\w+) = ([\w.]+):(0x[0-9A-Fa-f]+);(.*)$")
    with open(path) as f:
        for line in f:
            m = pat.match(line.strip())
            if not m:
                continue
            name, section, addr, rest = m.groups()
            tm = re.search(r"type:(\w+)", rest)
            sm = re.search(r"size:(0x[0-9A-Fa-f]+)", rest)
            info[name] = {
                "section": section,
                "addr": int(addr, 16),
                "type": tm.group(1) if tm else "?",
                "size": int(sm.group(1), 16) if sm else 0,
            }
    return info


def archive_defined():
    out = subprocess.run(
        ["nm", "-g", os.path.join(ROOT, "build", "libgame.a")],
        capture_output=True, text=True, errors="replace",
    ).stdout
    return {m.group(1) for m in re.finditer(r"^[0-9a-f ]* [TDBSC] _(\w+)$", out, re.M)}


def main():
    syms = load_undefined()
    info = load_symbols_txt()
    defined = archive_defined()

    groups = defaultdict(list)
    for s in syms:
        if s in defined:
            groups["already_defined_in_archive"].append(s)
            continue
        meta = info.get(s)
        if meta is None:
            groups["not_in_symbols_txt"].append(s)
        elif meta["type"] == "object":
            groups[f"data_{meta['section']}"].append(s)
        else:
            groups["function_no_c"].append(s)

    for g in sorted(groups, key=lambda g: -len(groups[g])):
        names = groups[g]
        total = sum(info[n]["size"] for n in names if n in info)
        print(f"{g:32s} {len(names):4d}  (data bytes: {total:#x})" if g.startswith("data") else f"{g:32s} {len(names):4d}")
        for n in sorted(names)[: (8 if not g.startswith('data') else 5)]:
            extra = f"  size={info[n]['size']:#x}" if n in info else ""
            print(f"    {n}{extra}")
        if len(names) > 8:
            print(f"    ... +{len(names) - 8} more")
    return 0


if __name__ == "__main__":
    sys.exit(main())
