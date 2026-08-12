# Foxhollow

Native port of Star Fox Adventures (GameCube) built on the SFA decompilation and Aurora.
The decomp lives at `~/Code/sfa` (github.com/zcanann/SFA-Decomp) and is the source of truth for
all game code; game-code fixes belong there first, port-layer code belongs here.

## Rules
- NEVER write code comments unless explicitly asked.
- NEVER commit game assets, disc images, or anything derived from the retail binary.
- NEVER add co-authored-by lines to commits.
- The repo is private until release; do not make it public or publish artifacts from it.

## Build
```sh
cmake -B build -G Ninja
cmake --build build --target foxhollow
```
First configure fetches Dawn — slow, expected.

## The bug class

Almost every bug here is the same shape: **the code is correct, the data is misread.**
Two rules define it.

**Only pointers changed width (4 → 8).** An `s16` is still an `s16`. So a struct's layout
only drifts *after* a pointer field — everything before it keeps its retail offset. Any code
still using a hardcoded retail offset then reads the wrong bytes.

**File-backed data is big-endian and is not swapped globally.** Placement data, tab files and
asset headers arrive raw. Each consumer swaps its own fields; `u8` fields read fine, which is
why these bugs present as "mostly works, one thing is insane".

Corollary: **"identical to the decomp" does not mean correct.** Decomp-faithful code that
hardcodes an offset or reads a BE float is still broken here. Diffing against `~/Code/sfa`
proves faithfulness, not correctness — and the decomp is ~100%, not 100%, so it can be wrong too.

Worked examples, both real:
- `loadObjectFile` swapped the `ObjDef` header but not the attach-point table it points at, so
  every `ObjPath_GetPointWorldPosition` returned garbage. `s8` joint indices needed no swap, so
  parenting worked and only path queries broke.
- `DustMoteSou` read its placement `f32 scale` unswapped: `0x7B10203E` = `7.7e+35` instead of
  `0x3E20107B` = `0.156`. A blinking jukebox button became a screen-filling white pane.

`STATIC_ASSERT` is a **no-op** outside MWERKS (`global.h`), so no struct layout is verified.
Enabling it as `_Static_assert` yields ~369 failures across ~20 structs; most are vtables and
name-accessed structs (harmless), but any struct read via `memcpy`, raw offsets, or straight
from file data is a real candidate. `grep` for raw-offset reads currently finds ~305 sites,
228 of them in `player.c`.

## Debugging tactics

Prefer these over reasoning from source. Reading the code proves what *should* happen;
these prove what *does*.

**Attach lldb to the running game.** The build is signed with `get-task-allow`
(`debug.entitlements` + a `POST_BUILD` codesign step), so this works without sudo.
- Get the real pid with `pgrep -x foxhollow` — `pgrep -f` matches the shell wrapper and the
  attach fails confusingly.
- **Never put `process continue` in an `lldb -b` batch.** If it hangs and the batch is killed,
  the debuggee dies with it. Attach → read → detach is instant and safe.
- Calling game functions works: `expr -- (void*)textureLoadAsset(0x5fd)`.
- `malloc` a scratch buffer, fill it in an `expr` loop, then `memory read` it — the way to dump
  arrays, since varargs `printf` inside a loop fails to compile in lldb.

**Freeze the process for unhurried inspection.** `kill -STOP <pid>` holds the frame and
survives lldb detach; `kill -CONT` resumes. Essential for transient objects. Each attach
advances a frame or two, so grab everything in one pass.

**Bisect by runtime gate, not by rebuild.** Add a `u32 gFhRenderGate` global, wrap each render
phase in `if (gFhRenderGate & (1u<<N))`, then flip bits live via lldb. One build, many
hypotheses, no replays. This localised both the projectile rosette and the title-screen pane in
a handful of observations after hours of guessing got nowhere.

**Disassemble the retail binary** to check the decomp itself:
```sh
llvm-objdump -d -r --triple=powerpc ~/Code/sfa/build/GSAE01/obj/<unit>.o
```
`~/Code/sfa/config/GSAE01/symbols.txt` maps names to addresses. This is how to answer
"is the decomp right here?" instead of assuming.

**Compare native vs retail offsets** with a throwaway program that prints `offsetof`, built with
the real flags (`-DTARGET_PC=1 -DVERSION_GSAE01` and `-include port/include/foxhollow_compat.h`).
Omitting `TARGET_PC` silently changes `u32` to 8 bytes and invalidates every number.

**Read disc data directly** — `~/Code/sfa/orig/GSAE01/files/` has the extracted files
(`TEXTABLE.bin`, `TEX0.tab`, …) for checking what an id *should* resolve to.

**Extract frames from screen recordings** with AVFoundation via `swiftc`, then crop/zoom, to
inspect an artifact frame by frame instead of relying on description.

## Gotchas that cost real time

- `OSReport` is a **weak no-op** in this build; Aurora's is `#if 0`'d. Use `fprintf(stderr, …)`
  for debug output or you will get silence and misread it as "code never ran".
- **Dev shortcuts manufacture phantom bugs.** The prologue quick-load in `gameloop_main.c`
  bypasses the title screen, so the level texture bank never merges and ids fall back to
  offset-0 garbage. Before deep-diving a rendering oddity, reproduce it on a normal boot.
- **Dedupe logs on every field you care about.** Keying on colour alone hid non-white quads;
  keying on texture alone hid varying colours. Both produced confident false negatives.
- Transient objects vanish between attaches — freeze first, or retry-loop the attach.
- Aurora is largely fine. Its `RGB5A3`/`CMPR`/`I8` decoders, indirect-TEV, blend and EFB-copy
  paths were all verified correct during hunts that turned out to be data bugs.

## References
- `extern/aurora` — the compatibility layer (see docs/ and examples/simple.c)
- `extern/borealis` — cross-platform port modules (logging, crash, data dirs, updates)
- TwilitRealm/dusklight — Twilight Princess port, same architecture; the working example to
  crib from (vendored decomp + `src/dusk` port layer + Aurora/Borealis submodules)
- HarbourMasters/Starship — Star Fox 64 port (N64 lineage, sibling game)
- docs/ROADMAP.md — the phase plan
