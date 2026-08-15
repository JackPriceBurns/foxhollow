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
Three rules define it.

**Only pointers changed width (4 → 8).** An `s16` is still an `s16`. So a struct's layout
only drifts *after* a pointer field — everything before it keeps its retail offset. Any code
still using a hardcoded retail offset then reads the wrong bytes.

**File-backed data is big-endian and is not swapped globally.** Placement data, tab files and
asset headers arrive raw. Each consumer swaps its own fields; `u8` fields read fine, which is
why these bugs present as "mostly works, one thing is insane".

**Packed scalar constants reinterpreted as byte structs are endian-dependent.** A retail-style
`u32 color = 0xRRGGBBAA` followed by `*(GXColor*)&color` has the intended channel order only on
big-endian hosts. Define the value as a `GXColor` with explicit fields, or unpack it with shifts.

**Hardware conversion intrinsics are behavioral contracts, not ordinary C casts.** GameCube
paired-single quantized stores and the Dolphin `OSFastCast` helpers saturate values to the target
integer range. Replacing them with `(s16)float`, `(s8)float`, or unsigned equivalents changes the
behavior for out-of-range values; the C conversion is undefined or implementation-defined and may
wrap to the opposite end of the range. Audit compatibility shims against the retail instruction,
especially around packed vertices, normals, colors, animation data, and fixed-point streams.

Corollary: **"identical to the decomp" does not mean correct.** Decomp-faithful code that
hardcodes an offset or reads a BE float is still broken here. Diffing against `~/Code/sfa`
proves faithfulness, not correctness — and the decomp is ~100%, not 100%, so it can be wrong too.

Worked examples, both real:
- `loadObjectFile` swapped the `ObjDef` header but not the attach-point table it points at, so
  every `ObjPath_GetPointWorldPosition` returned garbage. `s8` joint indices needed no swap, so
  parenting worked and only path queries broke.
- `DustMoteSou` read its placement `f32 scale` unswapped: `0x7B10203E` = `7.7e+35` instead of
  `0x3E20107B` = `0.156`. A blinking jukebox button became a screen-filling white pane.
- `SB_ShipHead` exceeded the signed-16 vertex range during its mouth-open animation. Retail
  `psq_st` saturated `33172.289` to `32767`; Aurora's `(s16)` cast wrapped it to roughly `-32364`,
  pulling a small group of ear vertices straight through the model until the animation moved back
  in range. The fix was a port-owned saturating `OSFastCast.h`, not a change to the game animation.
- `gBlurFilterKColor = 0x666666FF` was reinterpreted as a `GXColor`. Retail read gray RGBA
  `(0x66, 0x66, 0x66, 0xFF)`; the little-endian port read translucent red
  `(0xFF, 0x66, 0x66, 0x66)`, tinting the Ship Battle intro. Declaring it as `GXColor` fixed the
  channel order without disabling the depth-of-field blur.

### Four consequences of the 4 → 8 pointer growth

These are the same root cause wearing different clothes. All four are mechanically detectable.

**Pointers squeezed through 32-bit storage are fatal, not lossy.** `AllocMEM1` is
`calloc(1, 128MB)` (`OSMemory.cpp`), so MEM1 lands **above 4 GB** (observed `0xb32000000`).
Any pointer through an `int` truncates to an unmapped address — a guaranteed segfault the
moment that path runs. These are landmines: harmless until reached. Forms to hunt: `int`
local, `int` parameter, `int` return type, `int` struct field, `(u32)ptr != 0` null tests,
and vtable calls like `(*(int*)obj->anim.dll + 0x20)` that read the vtable *pointer* as `int`
**and** assume 4-byte entries. Convert vtable calls to indexed form — retail byte offset N
becomes index N/4: `((void (*)(GameObject*))obj->anim.dll[0][8])(obj)`.

**`getExtraSize` returning a hardcoded retail literal under-allocates.** If the state struct
holds a pointer it grew, but the allocation did not — every field past the pointer is written
outside the block. Silent heap corruption that surfaces as an unrelated crash later. Real:
`enemy_getExtraSize` returned `0x370` for a 0x388 struct; `Tricky_getExtraSize` returned
`0x83c` for a 0xA68 struct (0x22C past the end, on an always-resident object);
`DFSH_Shrine` returned `0x20` for 40 bytes because `ModelLightStruct* light` is its first
field. Fix is always `return sizeof(TheStruct);`. Audit by comparing each literal to a
measured `sizeof`.

**Global arrays sized by a hardcoded retail byte count under-allocate the same way.**
`getExtraSize` is not the only place a literal encodes a retail `sizeof`. Real, both in
`lightmap_initmapblocks.c`: `gMapTextureOverrides = mmAlloc(0x500)` is 80 × the *retail* 16-byte
`MapTextureOverride`, but `Texture* texture` is its first field so it is 24 bytes natively —
`updateEnvironment` walks all 80 every frame and read ~640 bytes past the end, crashing on a
garbage `texture` pointer. `gMapBlocks = mmAlloc(0x100)` holds **64 pointers**: 256 bytes was
right at 4 bytes each, but 64 × 8 = 512 natively, so `shader.c`'s `for (j = 0; j < 64; j++)
gMapBlocks[j] = NULL;` wrote 256 bytes past the end into the arrays allocated right after it.
Audit rule: any `mmAlloc(<literal>)` whose target is `T*` where `T` contains a pointer — or
`T**`, an array of pointers — is wrong. Sibling arrays give you the true element count for free
(`gMapBlockIds` is `0x80`/2 = 64, `gMapBlockRefCounts` is `0x40` = 64). Scalar-only structs
(`MapCellEntry`, `IntersectLine`, `MapTextureScroll`) are unchanged and must be left alone.
Fix with `count * sizeof(T)`.

**A union that gains a pointer member stops being 4 bytes, breaking implicit Vec3s.**
`TrickyScratch` holds a `GameObject*`, so it is 8 bytes; three "consecutive" scratch slots
written as X/Y/Z are then at stride 8 while every consumer reads them as a packed `f32[3]`.
Tricky's tumbleweed target was correct in X and garbage in Y and Z. Give such a target its
own `f32[3]`.

**Retail memory-map constants are meaningless here.** `if ((u32)ptr > 0x90000000)` guards
were dead code on GameCube (MEM1 is `0x80000000-0x817FFFFF`, so never above the watermark),
but truncating a 64-bit heap pointer makes them fire at random — and the full-width pointer
makes them fire *always*. Neither is retail behaviour. `fhAboveRetailMem1Watermark` in
`foxhollow_compat.h` encodes the correct answer (never) in one place.

Widening a shared scratch field is sometimes the *right* fix, not a layout hazard:
`GameObject.userData1/userData2` are documented general-purpose slots that several classes
use to stash a `GameObject*`. Widening both to `intptr_t` fixes every writer/reader pair at
once — including ones you have not found — where rewiring individual sites silently breaks
any pair you miss. Verify first that nothing reads the struct tail by raw offset and that
allocation is `sizeof`-based (it is: `object.c:1775`, `object.c:2101`).

### Placement data: the base header is swapped, the payload is not

`fhSwapRomListSection` (`port/src/game_endian.c`) walks the variable-length placement records
and swaps **only** the 24-byte base header — `objectId` at 0x00 and the u32s at 0x08/0x0c/0x10/0x14
(`posX/posY/posZ/ident`). **Everything at offset >= 0x18 — the object-specific payload — stays
big-endian.** So raw reads of `base.posY` or `base.ident` are correct, and raw multi-byte reads
of a per-class field are bugs. That is what `ObjAnim_ReadPlacement{U16,S16,S32,U32,F32}` is for;
it swaps unless `OBJANIM_FLAG_OWNS_PLACEMENT_DATA` marks a heap-allocated setup. `u8`/`s8`
fields need no swap. Watch for `(s8)` casts over an unswapped `s16` — they take the retail
*high* byte, so 5 reads as 0.

`RomCurveDef` is placement-shaped and follows the same rule: `RomCurve_add` swaps only
`linkIds`, so `requiredBit`/`forbiddenBit` at 0x30/0x32 need `fhReadBES16`. Unswapped, a valid
gamebit id like `0x04E2` reads as `0xE204` → `mainGetBit(-7676)`, and **every gated curve
evaluates as "never passable"**. `-1` is byte-symmetric, which is why the "none" sentinel kept
working and hid this.

### A fourth class: decomp-faithful C that is undefined behaviour

MWERKS had one evaluation order; clang has another. Code the decomp reproduces faithfully can
still be UB, and then it works on PowerPC and silently produces garbage here. `grep` will not
find these — **the compiler will**: `-Wunsequenced -Wuninitialized` over
`build/compile_commands.json` (strip ` -w ` first) currently reports ~52 sites.

Real, and it cost a long hunt: `atanf` (`game/src/main/acosf.c`) had
```c
squared = reduced * (reduced = 1.0 / absoluteValue);   /* reads and writes `reduced` unsequenced */
```
Clang evaluated the left operand first, reading `reduced` **uninitialised**, so `atanf` returned
**NaN** for every `|value| > 1`. NaN then poisoned frustum planes 1-3 in
`updateVisibleGeometry`, and since every comparison against NaN is false, `mapBlockIsInViewFrustum`
returned "outside" for *every* map block — the entire map vanished while objects, which cull
elsewhere, kept drawing. It presented as "see through the world into a blue void".

The tell was a **hard FOV threshold**: the broken branch is entered when
`sqrt(2.7778) * |tan(scale)| > 1`, i.e. **FovY > 61.93°** normally, or **> 46.45°** with
`RENDERFLAG_WIDESCREEN`/`RENDERFLAG_DRAW_DISTANCE` (which use `FovY/1.5` instead of `FovY*0.5`).
Normal gameplay sits under it; anything that widens the FOV — cutscene cameras, explosion punches —
crosses it and blanks the map until the FOV animates back down. Observed exactly: culled at
`FovY=74.5`, restored at `FovY=61.8`.

Fix is `reduced = 1.0 / absoluteValue;` on its own line. When a value is NaN rather than merely
wrong, suspect UB or a math shim before suspecting the data — this is the one bug class where the
"file-backed data is misread" instinct actively misleads.

Also endian-dependent: a native `u16[]` table whose entries are *reinterpreted* as `int` to
compose a 32-bit id (`DFSH_SHRINE_TARGET_OBJECT` read pairs at `+0x3C` as `int`; retail composes
`0x00049054`, the port composed `0x90540004`). Compose with explicit shifts instead.

**The nastiest endian case: one pointer, two provenances.** When a pointer can reference *either*
file data *or* a native local/global, a blanket `fhReadBE*` conversion is right for one and wrong
for the other. On GameCube both are big-endian, so the decomp's plain `*(s16*)p` is correct for
both and the bug is invisible to any diff. Real: `ObjModel_ApplyBlendChannels` uses
`targetA/targetB = (u8*)&defFrame` as a "no morph target" sentinel, where `defFrame` is a **native
local** holding `vertexCount + 1`; `modelApplyBoneTransform` then reads it as
`(fhReadBES16(b) & 0x1fff)`. For a 1120-vertex model the sentinel `0x0461` (1121, deliberately
unreachable) was read byte-swapped as `0x6104 & 0x1fff` = **260**, so from vertex 260 on the code
took the "apply target B" branch and decoded **stack memory** as morph deltas — deterministic
garbage on a subset of vertices, scaled by the animated blend weight. It presented as EarthWalker
head vertices pinging off and returning on the animation loop. Fix: store the sentinel pre-swapped
(`fhSwap16(vertexCount + 1)`) so the big-endian read decodes it correctly.
Grep shape: `(u8*)&someLocal` / `(u8*)&gSomeNativeTable` assigned to a pointer that is later read
with `fhReadBE*` or walked as file data (13 such sites exist; the rest are native global tables).

Two near-misses from that same hunt, both worth not "fixing":
`((u32)(ax * wHi + bx * f) >> 16)` looks like an unsigned-shift bug on a signed product, but
retail uses `srwi` too (0 `srawi` in `modelApplyBoneTransform`) and only the low 16 bits are
stored via `sth`, so logical and arithmetic shifts are equivalent there. And the morph cache's
`in`/`out` regions sit only `0x1000` apart, which looks like an overlap — but
`gModelCopyChunkWordLimit = 0x2A0` caps a chunk at 672 vertices × 6 = 4032 bytes precisely to
prevent it. Verify against retail asm before changing either.

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

**Use the compiler as an exhaustive detector, not grep.** `-w` (`CMakeLists.txt`) hides the
entire truncation class from normal builds, which is why these bugs are found one crash at a
time. Strip it per file and ask directly:
```py
cmd = entry['command'].replace(' -w ', ' ') + \
      " -Wint-conversion -Wint-to-pointer-cast -Wpointer-to-int-cast -fsyntax-only"
```
over `build/compile_commands.json`. This finds every site in a file in one pass and gives a
zero-warnings completion criterion. Regexes miss the `int`-local and `int`-parameter forms
entirely. Caveat: a warning-free file is not a clean file — raw-offset reads, endian bugs and
hardcoded allocation sizes are all invisible to the compiler and need the harnesses above.

**Retail relocations are ground truth for function tables.** When code indexes a table by byte
offset from a neighbouring symbol, `llvm-objdump -r` names the target directly — no inference:
```sh
llvm-objdump -r --triple=powerpc ~/Code/sfa/build/GSAE01/obj/<unit>.o
```
This is how Tricky's dispatch base was settled: the relocation at `.data+0x6C` is literally
`tricky_substateFollowIdle`, proving the `+0x6c` dispatch starts at handler index 18. A run of
`R_PPC_ADDR32` every 4 bytes also tells you a region is a pointer table and how many entries it
has — which is how you catch a table indexed past its end at the wrong stride.

**Compare native vs retail offsets** with a throwaway program that prints `offsetof`, built with
the real flags (`-DTARGET_PC=1 -DVERSION_GSAE01` and `-include port/include/foxhollow_compat.h`).
Omitting `TARGET_PC` silently changes `u32` to 8 bytes and invalidates every number.

**Resolve what a map actually loads before auditing it.** Guessing from directory names is
unreliable — many `WM_*` DLLs are cut content that never instantiates. Decode the romlist:
`~/Code/sfa/orig/GSAE01/files/<dir>/<romlist>.romlist.zlb` is a 16-byte BE header then zlib;
records are `>h` placementId, `B` sizeWords, `B` flags, with `s32 ident` at +0x14. Map
placementId through `OBJINDEX.bin` (`>h`, −1 = identity), then the DLL slot is
`OBJECTS.bin[off+0x50]` (`>H`), which is the directory number in `game/src/dlls/dlls.txt`.
Krazoa Palace is map `0x0B`, romlist `warlock` (originally "Warlock Mountain").

**Read disc data directly** — `~/Code/sfa/orig/GSAE01/files/` has the extracted files
(`TEXTABLE.bin`, `TEX0.tab`, …) for checking what an id *should* resolve to.

**Replay asset math without rendering.** For intermittent deformation bugs, load the exact model
and animation assets into a small native harness, parse the display-list vertex-to-matrix bindings,
then sweep every animation frame, transition weight, morph target, and CPU skinning chunk. Inspect
the floating-point value immediately before packed conversion and compare the port result with the
retail instruction's range behavior. A useful regression must fail under the old shim and pass under
the fix; bounded final poses alone can miss a one-instruction packing mismatch.

**Extract frames from screen recordings** with AVFoundation via `swiftc`, then crop/zoom, to
inspect an artifact frame by frame instead of relying on description.

**Drive the game yourself instead of using the user as an input device.** `FOXHOLLOW_AUTO_A=1`
makes `padUpdate` (`game/src/main/pad.c`) synthesize an A press for 6 of every 45 frames, which
is enough to walk the title screen → save select → load, and to trigger any A-activated object.
With a save parked next to the thing you are debugging, one `nohup ./build/foxhollow <iso> &`
plus a log grep is a complete reproduce-measure cycle with no human in the loop — turn a
ten-minute-per-iteration hunt into a thirty-second one. Audio defaults to muted in
`FOXHOLLOW_DEBUG_SHORTCUTS` builds (`port/src/ai_shim.c`, `m` toggles), so unattended runs are
silent. Add more synthetic buttons the same way when a repro needs them.

**Reset per-frame counters inside the log window, not at declaration.** A counter that starts
accumulating at program start reads as a huge first sample and invites a wrong conclusion —
this cost real time when 681 render calls in the first logged frame looked like the anomaly and
was just the boot total. Log the delta, and sanity-check a counter's magnitude against what one
frame can plausibly produce before theorising about it.

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
