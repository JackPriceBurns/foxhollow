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

## Workflow

- `tools/compile_progress.py` sweeps all game TUs with `-fsyntax-only`; `--update` rewrites
  `port/compiling.txt`, which CMake reads as the `game` library's source list. Flags live in
  both the script and `CMakeLists.txt` — keep them in sync.
- `game/` is a deliberately diverging fork of the decomp pinned in `game/VENDORED.txt`.
  Game-code fixes that aren't port-specific belong upstream in the decomp first.
