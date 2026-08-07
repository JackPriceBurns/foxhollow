# Porting notes — semantic decisions

Decisions made while getting the vendored decomp compiling under clang. Each entry lists the
change and what to re-audit later.

## Types

- **`bool` is now `<stdbool.h>`** (game `include/types.h` used `typedef int bool;`). Size drops
  4 → 1. Any disc-loaded struct with a `bool` field changes layout — audit during the Phase 3
  endianness pass alongside offsets.
- **`u32` et al. come from Aurora's `dolphin/types.h`** (stdint-based). The game's own
  `include/dolphin` was deleted at vendor time; Aurora's dolphin headers are canonical, with
  `port/include` shims filling gaps (see `port/include/dolphin/`).
- `TARGET_PC=1` must be defined globally — Aurora's `types.h` gates `BOOL`/`TRUE`/`FALSE` and
  `<stdbool.h>` behind it.

## Math and intrinsics (`port/include/foxhollow_compat.h`, force-included everywhere)

- `__frsqrte` is implemented as exact `1.0/sqrt(x)`. Hardware gives an estimate that game code
  refines with Newton iterations, so port results are slightly *more* precise. Revisit only if
  physics or recorded-input playback diverges.
- `fastCast*` (paired-single/FPSCR conversion tricks) are plain C conversions.
- `__cvt_fp2unsigned` saturates (negative → 0, ≥2³² → 0xffffffff), matching PPC conversion
  saturation semantics.
- The decomp repurposes MSL header names (`dolphin/MSL_C/.../math_api.h` etc.) as game-math
  declaration headers. The port versions in `port/include` keep the game declarations and drop
  MWCC-specific bits.

## GX

- Local MWCC absolute-address declarations of the write-gather pipe
  (`volatile PPCWGPipe GXWGFifo : (0xCC008000);`) were deleted from game TUs; Aurora's
  `GXVert.h` provides `GXWGFifo` as a macro on PC.
- A few GX functions missing from Aurora's headers are declared in `port/include/dolphin/gx/`
  overlay headers (`GXLoadTexObjPreLoaded`, `GXPreLoadEntireTexture`, `GXGetLightAttnK`,
  `GXSetTevIndRepeat`, `GXGetScissor`, `__GXAbortWaitPECopyDone`). These compile but are NOT
  implemented by Aurora yet — they need port-side implementations or stubs before link/runtime.
- Aurora's granular gx sub-headers assume inclusion via `gx.h`; overlays pre-include
  `GXEnum.h`/`GXStruct.h` so the game's direct granular includes work.

## Call-site corrections

- `Sfx_PlayFromObject((int)obj, ...)` casts removed (6 files) — pointer-truncating on 64-bit.
  The real signature is `void Sfx_PlayFromObject(GameObject*, u16)`; the `(GameObject*, int)`
  prototype in `sfx_play_legacy_api.h` is a decomp-matching artifact.
- MWCC (C89) accepted implicit function declarations; clang gnu11 makes them errors — kept as
  errors deliberately because implicit float/pointer args are a real ABI hazard on arm64.
  Fixes add the correct existing game header include, or an extern prototype matching the
  definition.

## Symbol and API corrections

- **Game `rand`/`srand` renamed `sfaRand`/`sfaSrand`** (defined in `main/rand.c`, callers in
  `vecmath.c`, `dlls/engine/7`, `dlls/engine/14`). The game's LCG would otherwise shadow libc
  `rand` for the whole binary, including Aurora/SDL internals.
- **musyx cross-TU statics de-static'd**: `snd3d.c` calls functions defined `static` in
  `snd3d_calc.c`/`snd3d_room.c` (`CalcEmitter`, `AddStartingEmitter`, `AddRunningEmitter`,
  `SetFXParameters`, `StartContinousEmitters`, `CheckRoomStatus`, `CheckDoorStatus`) — a decomp
  TU-split artifact that cannot link as standard C. Made external with prototypes in `snd3d.c`.
- **`CARDInit("GSAE", "01")`** — Aurora's PC CARD API takes game/maker codes for save naming;
  the SDK version took none.
- **ARQ callbacks widened** to Aurora's `ARQCallback(uintptr_t)` in `main/audio.c` and
  `musyx/runtime/aram_queue.c`.
- **Pointer-in-`u32` tables widened to `uintptr_t`**: `main/texture.c` (`lbl_8030DC88`),
  `dlls/engine/0/0.c` (`lbl_8031AF14`, self-referential), `dlls/engine/11/11.c`
  (`Dll0BDescriptorTable.words`). Element size doubles — audit consumers during Phase 4 DLL
  work.
- `main/object.c` `Obj_IsLoadingLocked`: MWCC `asm` block converted to equivalent C
  (`(getLoadedFileFlags(0) & 0x100000) == 0`).
- `dlls/engine/67/67.c` `CameraModeStaffAnim_samplePath`: bare `return;` in a non-void function
  now returns the `Curve_AdvanceAlongPath` end-of-path result, reconstructed from caller usage —
  re-verify against retail behavior when the camera runs.
- `dlls/objects/328_CFGuardian/CFGuardian.c`: pointer-bearing `int [33][2]` sequence table
  rewritten as a typed struct table (unreferenced rows dropped, no-op `seqPairTablePrepare` call
  removed) — re-audit against retail behavior.

## Known exclusions (7 TUs off `port/compiling.txt`)

- `main/boot_logo.c` includes `gLoadingScreenTextures.inc`, generated from game assets at build
  time — excluded until the Phase 3 asset pipeline exists.
- `main/objprint_dolphin.c`, `main/tex_dolphin.c`, `dlls/engine/19/19.c`: SDK 3-arg
  `GXSetArray` vs Aurora's PC 5-arg form (needs buffer size + endianness) — Phase 2 decision.
- `main/pi_dolphin.c`, `main/dll_80136a40.c`: poke `OSContext.gpr/srr0`; Aurora's PC OSContext
  is opaque storage — needs a port accessor story (Phase 2).
- `dlls/objects/315_WallAnimato/WallAnimato.c`: `objAddObjectType`-family takes `int` object
  handles holding pointers — needs a port-wide handle widening (Phase 2/4; see also
  `seqPairTableLookup` and `ObjSeqStreamMapEntry` in `dlls/engine/2`).

## Phase 2 — SDK shims and the full link

- **`port/src/` shim layer** covers everything Aurora doesn't implement: OS interrupts (single-
  threaded model), threads (recorded, not started — loud stub), message queues (functional ring
  buffer, non-blocking), stopwatches (real, over OSTime), fonts (stubs), reset/sound/progressive
  modes, DC/LC cache no-ops, AI (state-recording stubs — SDL audio wiring comes in Phase 5), DSP
  (mail always empty; tasks not executed), PPC register intrinsics (zeros), VI retrace/framebuffer
  family (callbacks fire from VIWaitForRetrace), THP decode (stubs; movies skipped), missing GX
  entries (draw-sync token loopback, preload no-ops, GXNtsc480Prog render mode).
- **`port/src/game_data_gen.c`** is GENERATED by `tools/gen_data_defs.py`: symbol addresses/sizes
  from the decomp's `symbols.txt`, bytes from the user's DOL, byteswapped by element width
  (209 initialized symbols, 27 zero-init, `gLoadingScreenTextures` as a zeroed 256KB placeholder
  — the real texture is an asset and is never embedded). Pointer-bearing data would be skipped
  and reported; currently none are.
- **`port/src/game_math_port.c`**: the game's `mathSinf`/`mathCosf`/`powfCore`/`log2fBitEstimate`
  family has no C upstream (not yet decompiled); portable libm implementations serve instead.
  `trigReduceQuadrant` is mirrored from the decompiled HighPrecision variant.
- **`sqrtf`**: all five game-local definitions removed (the `extern inline` volatile-slot idiom
  plus one real definition); libm's sqrtf serves. The game's Newton-refined estimate converges to
  libm values anyway.
- **`modelAnimBuildJointMatrices` is a loud stub** — the function is not decompiled upstream.
  Skeletal animation is broken until it's implemented. This is the #1 decomp gap for runtime.
- `ReverbSTDCreate`/`ReverbSTDCallback` stubbed until Phase 5 audio.
- `musyx/runtime/snd3dgroup.c` is an amalgamation TU (#includes three fragment .c files); the
  fragments are excluded from standalone compilation via `port/exclude.txt`.
- The `game` target compiles with `-fcommon` (MWCC-era tentative-definition semantics).
- **The game's `main` (gameloop_main.c) is renamed `gameMain`** — the port's entry point owns the
  process; Phase 3 boot work calls `gameMain`'s logic from inside the Aurora frame loop. (Before
  the rename, force-loading made the game's main the process entry — and it ran and exited
  cleanly on macOS, which bodes well.)
- **Link tooling**: the `linkcheck` CMake target force-loads all of `libgame.a`;
  `tools/link_census.py` clusters undefined symbols by subsystem; `tools/classify_undefined.py`
  splits game symbols into data sections vs functions using the decomp's symbols.txt.

## Phase 3 — boot-crawl decisions

- **Entry architecture**: the port's `main` initializes Aurora, opens the user's disc image via
  `aurora_dvd_open`, then calls the game's `gameMain`. The game's own loop stays in charge;
  `VIWaitForRetrace` (port shim) is the frame pump — it ends the Aurora frame, processes events
  (window close exits), begins the next frame, and fires the game's retrace callbacks.
- **MEM1 is 64MB** (GC had 24MB): 64-bit pointers and structs inflate the game's memory layout;
  the retail carve-up (mmInit regions + heap headers) overflows 24MB.
- **GXWGFifo is unusable on PC** — Aurora's PC contract is typed vertex submission. All 697
  direct pokes across 15 TUs were converted to GXPosition3f32-family calls (attribute mapped
  from each site's vertex descriptor); raw BP/XF command writes (GPU metrics, hang recovery in
  pi_videoinit.c) were deleted as PC-meaningless. PNMTXIDX bytes go through GXPosition1x8
  (byte-stream identical).
- **Pointer-through-int fixes** (the recurring 64-bit disease): videoInit/50.c arena alignment
  casts; mm.c allocator internals (mmAllocFromRegion returns uintptr_t); musyx
  `salAiDmaBuffer`; ARQ upload passes the real pointer to Aurora's widened ARQPostRequest;
  `gMapRomListBuffers`/`gResourceFileBuffers` arrays; generated zero-init data symbols are
  over-allocated (2×, min 16B) because several hold host pointers now.
- **mm.c `MmGlobalLayout` overlay removed**: retail aliased separate globals at fixed byte
  offsets from `gMmStoreArray` (0x3F00 → region table); the port references the real symbols.
  The layout STATIC_ASSERTs compile to no-ops on PC, which is why it failed silently.
- **DSP handshake is synchronous**: DSPAddTask fires the task's `init_cb` immediately; musyx's
  clear-flag-then-wait ordering in `salInitDsp` was swapped (hardware fired the callback via
  interrupt after the clear; the swap is behavior-identical on GC).
- **DVDFileInfo pool slots** use `sizeof(DVDFileInfo)` (GC hardcoded 0x40; Aurora's is larger —
  pool overflow wrote into read-only pages).
- **The IPL ROM font doesn't exist on PC**: `gameTextBuildSystemFontAtlas` bails when OSLoadFont
  reports no font; debug text is dark until a replacement font ships.
- **Main thread**: OSGetCurrentThread returns a static thread object with state RUNNING.
- The one direct low-memory read (`*(u32*)0x800000f8`, bus clock) uses OS_BUS_CLOCK.
- **pi_dolphin.c fully de-truncated** (agent audit): 508 strict int↔pointer conversion errors →
  0; the `MldfTables`/`MldfNames` GC-address overlays eliminated in favor of the real symbols
  (offset equivalences verified against decomp symbols.txt); in-flight `DVDFileInfo*` moved to
  a typed side array; the video flip queue carries `void*[3]` elements.
- **hudDrawColored takes `Texture*`** (was pointer-as-int). Two upstream chains still carry
  textures as u32 and only bridge via casts at their call sites: `getReflectionTexture1()`
  (newshadows) and `getObjectShadowDrawParams`'s d1 out-param (objprint) — widen these when
  their code paths run (save UI, object shadows).

## Where the boot-crawl stands

Boot progresses through: Aurora init → disc mount → full game `init()` — video, 64MB arena,
heap regions, loading-screen textures, camera, text renderer (font atlas skipped), controllers,
complete MusyX audio init (synchronous DSP), `initLoadFiles` disc reads, shadow procedural
textures — **and draws the loading screen** (`runLoadingScreens` → `hudDrawColored`). Current
frontier: the game's video-flip/DVD thread paths under the single-threaded model
(`OSCreateThread`/`OSSleepThread` stubs) — next session decides whether to implement real
threads (SDL) or a cooperative pump.

## Workflow

- `tools/compile_progress.py` sweeps all game TUs with `-fsyntax-only`; `--update` rewrites
  `port/compiling.txt`, which CMake reads as the `game` library's source list. Flags live in
  both the script and `CMakeLists.txt` — keep them in sync.
- `game/` is a deliberately diverging fork of the decomp pinned in `game/VENDORED.txt`.
  Game-code fixes that aren't port-specific belong upstream in the decomp first.
