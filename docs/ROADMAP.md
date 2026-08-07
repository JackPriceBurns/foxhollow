# Foxhollow roadmap

The model is Dusklight (Twilight Princess) and Party Board (Mario Party 4): vendor the
decompilation's game code, replace the Dolphin SDK layer with Aurora, shim what Aurora doesn't
cover, and read assets straight from the user's own disc dump.

## Phase 0 — Boot *(this scaffold)*

Aurora window opens, GX clear color renders, event loop runs. Proves the toolchain and the
Dawn/WebGPU build on this machine.

## Phase 1 — Vendor the decomp

Bring `src/main`, `src/dlls`, `src/track`, `src/musyx`, and `include/` over from SFA-Decomp.
Drop `src/dolphin` entirely — Aurora provides GX/PAD/DVD/CARD/VI; the rest gets shims.
Goal is *compiles under clang*, not runs: fix MWCC-isms, alignment and pointer-width
assumptions, SJIS literals, and anything relying on big-endian layout at compile time.

## Phase 2 — SDK shims

Thin portable implementations for what Aurora doesn't cover: OS (threads, alarms, arena),
DSP/AI/ARAM (stub first, real later), EXI/SI, GBA link (stub). Aurora's `mem1Size`/`mem2Size`
emulated regions back the game's arena so fixed-address assumptions have somewhere real to live.

## Phase 3 — Disc I/O and asset loading

Aurora's DVD layer (nod) reads the user's ISO/RVZ directly — no extraction step. Every asset
loader gets an endianness pass: file tables, MAP/screen data, models, textures, animations,
gamebits, savegames, and the DLL images. This is the long tail of the port.

## Phase 4 — The DLL system

SFA loads relocatable DLLs from disc at runtime. All of them exist as C in the decomp, so they
compile straight into the binary; the DLL manager's load/relocate path becomes a lookup into a
static descriptor table. The `modules/` DLL-vs-DOL split disappears as a runtime concept.

## Phase 5 — Audio

MusyX is decompiled in the source tree. Re-plumb its DSP/ARAM output path into SDL audio with a
software mixer standing in for the DSP. AxioDL's `amuse` (portable MusyX from Metaforce) is the
fallback if driving the original synth proves unreasonable.

## Phase 6 — It's a port now

Widescreen, uncapped/high frame rate (decouple the logic tick), Dolphin-format texture pack
support (Aurora built-in), controller remapping, save management, Borealis modules (crash
reporting, data dirs, update checks).

## Phase 7 — Platforms

Windows/Linux/macOS first-class, then Android and iOS via Aurora's platform layers.

## Non-goals

- Shipping any game assets or any form of the retail binary
- Emulation — this is a source port
- Supporting anything but a user-provided retail dump
