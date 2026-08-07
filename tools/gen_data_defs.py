#!/usr/bin/env python3
import os
import re
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SFA = os.path.expanduser("~/Code/sfa")
DOL = os.path.join(SFA, "orig", "GSAE01", "sys", "main.dol")
OUT = os.path.join(ROOT, "port", "src", "game_data_gen.c")

MANUAL = {"gWallAnimatorObjDescriptor", "waterfx_funcs"}
ASSET_PLACEHOLDER = {"gLoadingScreenTextures"}
ZERO_SECTIONS = {".bss", ".sbss", ".sbss2"}
PTR_LO, PTR_HI = 0x80000000, 0x81800000


class Dol:
    def __init__(self, path):
        d = open(path, "rb").read()
        offs = struct.unpack(">18I", d[0x00:0x48])
        addrs = struct.unpack(">18I", d[0x48:0x90])
        sizes = struct.unpack(">18I", d[0x90:0xD8])
        self.sections = [
            (addrs[i], sizes[i], offs[i]) for i in range(18) if sizes[i] > 0
        ]
        self.data = d

    def read(self, addr, size):
        for saddr, ssize, soff in self.sections:
            if saddr <= addr and addr + size <= saddr + ssize:
                off = soff + (addr - saddr)
                return self.data[off : off + size]
        return None


def load_symbols_txt():
    info = {}
    pat = re.compile(r"^(\w+) = ([\w.]+):(0x[0-9A-Fa-f]+);(.*)$")
    with open(os.path.join(SFA, "config", "GSAE01", "symbols.txt")) as f:
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


def undefined_game_syms():
    import glob as _glob
    out = subprocess.run(
        ["nm", os.path.join(ROOT, "build", "libgame.a")],
        capture_output=True, text=True, errors="replace",
    ).stdout
    refs = {m.group(1) for m in re.finditer(r"^ +U _(\w+)$", out, re.M)}
    defs = {m.group(1) for m in re.finditer(r"^[0-9a-f ]* [TDBSCtdbsc] _(\w+)$", out, re.M)}
    others = [os.path.join(ROOT, "build", "libport_shims.a")]
    others += _glob.glob(os.path.join(ROOT, "build", "extern", "aurora", "**", "*.a"), recursive=True)
    for lib in others:
        if not os.path.exists(lib):
            continue
        out2 = subprocess.run(["nm", lib], capture_output=True, text=True, errors="replace").stdout
        skip_member = False
        for line in out2.splitlines():
            mh = re.match(r"(?:.*\()?([\w.-]+\.o)\)?:$", line)
            if mh:
                skip_member = "game_data_gen" in mh.group(1)
                continue
            if skip_member:
                continue
            m = re.match(r"^[0-9a-f ]* [TDBSCtdbsc] _(\w+)$", line)
            if m:
                defs.add(m.group(1))
    return sorted(refs - defs)


def emit_zero(name, size):
    padded = max(16, size * 2)
    return f"u8 {name}[{padded:#x}] __attribute__((aligned(8)));\n"


def emit_init(name, raw):
    size = len(raw)
    if size % 4 == 0:
        words = struct.unpack(f">{size // 4}I", raw)
        if size == 4:
            return f"u32 {name} = {words[0]:#010x};\n"
        body = ", ".join(f"{w:#010x}" for w in words)
        return f"u32 {name}[{size // 4}] = {{{body}}};\n"
    if size % 2 == 0:
        halves = struct.unpack(f">{size // 2}H", raw)
        if size == 2:
            return f"u16 {name} = {halves[0]:#06x};\n"
        body = ", ".join(f"{h:#06x}" for h in halves)
        return f"u16 {name}[{size // 2}] = {{{body}}};\n"
    if size == 1:
        return f"u8 {name} = {raw[0]:#04x};\n"
    body = ", ".join(f"{b:#04x}" for b in raw)
    return f"u8 {name}[{size}] = {{{body}}};\n"


def pointer_suspect(raw):
    if len(raw) % 4 != 0:
        return False
    for (w,) in struct.iter_unpack(">I", raw):
        if PTR_LO <= w < PTR_HI:
            return True
    return False


def main():
    dol = Dol(DOL)
    info = load_symbols_txt()
    out_defined = subprocess.run(
        ["nm", "-g", os.path.join(ROOT, "build", "libgame.a")],
        capture_output=True, text=True, errors="replace",
    ).stdout
    defined = {m.group(1) for m in re.finditer(r"^[0-9a-f ]* [TDBSC] _(\w+)$", out_defined, re.M)}

    lines = ["#include <dolphin/types.h>\n\n"]
    skipped = []
    counts = {"zero": 0, "init": 0, "placeholder": 0}

    for name in sorted(undefined_game_syms()):
        if name in defined or name in MANUAL:
            continue
        meta = info.get(name)
        if meta is None or meta["type"] != "object":
            continue
        size = meta["size"]
        if size == 0:
            skipped.append((name, "zero size"))
            continue
        if name in ASSET_PLACEHOLDER:
            lines.append(f"u8 {name}[{size:#x}] __attribute__((aligned(32)));\n")
            counts["placeholder"] += 1
            continue
        if meta["section"] in ZERO_SECTIONS:
            lines.append(emit_zero(name, size))
            counts["zero"] += 1
            continue
        raw = dol.read(meta["addr"], size)
        if raw is None:
            skipped.append((name, "address not in DOL"))
            continue
        if pointer_suspect(raw):
            skipped.append((name, "pointer-bearing"))
            continue
        lines.append(emit_init(name, raw))
        counts["init"] += 1

    with open(OUT, "w") as f:
        f.writelines(lines)

    print(f"wrote {OUT}: {counts['init']} initialized, {counts['zero']} zeroed, {counts['placeholder']} placeholders")
    for name, why in skipped:
        print(f"SKIPPED {name}: {why}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
