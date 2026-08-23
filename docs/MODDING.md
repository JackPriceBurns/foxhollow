# Mod support

Asset mods work today — see [Asset mods](#asset-mods-shipped) for the shipped format. Native code mods
work as a [proof of concept](#code-mods-proof-of-concept). The rest of this document records what one
has to hook, and assesses a backlog of proposed mods against what the engine actually does. Findings are traced to source rather than
assumed; where something is a hypothesis it says so and gives the experiment. One backlog item —
frame pacing — turned out to be a port bug rather than a mod and has been fixed; it is kept below
because the other items depend on it.

Two conclusions up front:

- The interesting mods are **code hooks, not data files.** An asset-replacement system alone
  cannot express a mirrored world, a second player, or a race against a ghost. Designing the mod
  layer purely around file overrides would make every flagship example impossible.
- **The engine is already a plugin system.** SFA shipped its object classes as relocatable DLLs
  behind a vtable, and Phase 4 turned that into a static table rather than removing it. Exposing
  that table is the highest-leverage single thing the mod layer can do.

## Asset mods (shipped)

A mod is a directory with a `mod.json`. No code, no build step, no compiler.

```
mods/
  my-mod/
    mod.json
    textures/     replacement textures, matched by hash
    overlay/      files that replace or add to the disc, by path
```

`mods/example-texture-pack` is a working one: it repaints Fox's character atlas, which is visible
on the title screen the moment the game starts.

### Where the mods directory lives

`--mods <dir>`, then `FOXHOLLOW_MODS`, then `<Aurora user path>/mods`. The first two are explicit,
so a path that is not a directory is reported rather than ignored.

### mod.json

```json
{
  "id": "dev.example.my-mod",
  "name": "My Mod",
  "version": "1.0.0",
  "author": "You",
  "description": "Shown by the launcher.",
  "enabled": true
}
```

`id` is the only required field. `enabled: false` keeps a mod installed but inert.

### Load order

Mods load in case-insensitive directory-name order. When two mods overlay the same disc path the
later one wins and the loader logs which mod overrode which. Texture replacements take the same
order as ascending Aurora replacement priority, so later mods win there too.

### textures/

Scanned recursively. Filenames follow Dolphin's convention,
`tex1_{width}x{height}_{texhash}[_{tluthash}]_{format}.png|.dds`, where the dimensions and hashes
describe the *original* texture, not the replacement, and `$` is a hash wildcard. `_mipN` sidecars
next to a file are picked up automatically.

Matching happens on the hash of the decoded GX texture, so replacing a texture never requires
unpacking `TEX0.tab` or any other archive.

To find the filename for a texture, boot with `FOXHOLLOW_TEXTURE_DUMPS=1`. Every texture the game
binds is written to `<Aurora cache path>/texture_dumps` as an editable 32-bit RGBA `.dds`, already
named the way a replacement has to be named. `tools/make_example_texture.py` generates the example
pack's image if you want a starting point that is not derived from the disc.

### overlay/

Every file under `overlay/` replaces the disc file at the matching path: `overlay/audio/data/Music.bin`
serves `/audio/data/Music.bin`. Paths that do not exist on the disc are added as new files. Matching
is case-insensitive, and dotfiles and dot-directories are not packaged.

This covers every asset the game loads, because every read bottoms out in `DVDOpen` — `loadFileByPath`
(`game/src/main/fileio.c`), the `sResourceFileNameTable` archive opens (`game/src/main/pi_dolphin.c`),
audio, text and THP movies all go through it.

### Implementation

`port/src/foxhollow_mods.cpp`, called from `src/main.c` once the disc has been opened and validated,
and torn down on the quit path in `port/src/vi_shim.c`. It is a thin layer over two Aurora
subsystems that already existed and were previously unused by Foxhollow:

- `aurora_dvd_overlay_callbacks` / `aurora_dvd_overlay_files` (`extern/aurora/lib/dolphin/dvd/fst.cpp`)
  rebuild the disc FST with the overlay set. The whole set is registered in one call, so the loader
  merges every mod's files and resolves conflicts before registering.
- `aurora::texture::load_replacement_directory` (`extern/aurora/include/aurora/texture.hpp`) owns
  texture matching, decoding and priority.

Overlay reads are served by `fopen`/`fread` on a per-handle `FILE*`, because Aurora may call the
overlay callbacks from any thread.

Loader output goes to stderr. `stdout` is block-buffered when redirected, so a `stdout` message
appears out of order or not at all in a captured log — the same trap `OSReport` sets elsewhere.

## Code mods (proof of concept)

Working, and deliberately incomplete. `mods/example-code-mod` loads a native library, calls game
functions and interposes an object class callback to force the camera FOV to 90°. Treat the ABI as
unstable.

A mod with code adds one directory to the asset layout:

```
my-mod/
  mod.json
  lib/
    macos-arm64/mod.so
    linux-amd64/mod.so
    windows-amd64/mod.dll
```

The loader picks the directory matching the host and ignores the rest, so a mod that only ships one
platform loads on that platform and is skipped elsewhere. `.so` is used on macOS as well as Linux,
matching what CMake produces for a `MODULE` library.

### Lifecycle

`port/include/foxhollow_mod_api.h` is the whole ABI. A mod exports three functions:

```c
FH_MOD_EXPORT int  fh_mod_initialize(FhMod* mod, const FhModHost* host);
FH_MOD_EXPORT void fh_mod_update(FhMod* mod);
FH_MOD_EXPORT void fh_mod_shutdown(FhMod* mod);
```

Only `fh_mod_initialize` is required. It receives an `FhModHost` table carrying `modId`, `modDir`,
`log`, `frameCount`, `classCount` and `classReplaceCallback`, plus `structSize` and `abiVersion` so a
mod can refuse a host it was not built against. Returning anything but `FH_MOD_OK` disables the mod
without stopping the game. `fh_mod_update` runs once per presented frame from `VIWaitForRetrace`.

### Calling game code

Nothing special is needed on macOS: the game binary already exports 27,641 symbols in its Mach-O
export trie, so a mod links against it with `-bundle_loader` and declares what it wants.

```c
extern float Camera_GetFovY(void);
extern void Camera_SetFovY(float fovY);
```

Linux resolves the same way at `dlopen` time; `ENABLE_EXPORTS` on the `foxhollow` target supplies the
`-rdynamic` that needs. Windows cannot do this — a DLL needs an import library, which means generating
a `.def` for the executable link. That is not done yet, so Windows code mods do not work.

**A mod that includes game headers must be built with the same toolchain flags as the game**:
`-DTARGET_PC=1 -DVERSION_GSAE01`, `-fsigned-char`, `-fcommon`, and `-include port/include/foxhollow_compat.h`.
`TARGET_PC` is Aurora's, not Foxhollow's — the game inherits it from `aurora::core` — and without it
Aurora's `dolphin/types.h` defines `u32` as `unsigned long`, 8 bytes on LP64, so every struct offset the
mod computes silently disagrees with the host. The example mod sidesteps this by declaring the handful
of scalar functions it calls, which is the safe pattern until the SDK sets those flags for the author.

### Interposing an object class

`gResourceDescriptors[]` holds 705 `ResourceDescriptor*`, and a descriptor's callback array *is* the
object vtable. `ResourceDescriptor` is four `u32` of metadata followed by `acquire` and `release`, so
the interface sits 32 bytes in natively and `ObjectInterface`'s eight slots follow from there:
`slot02`, `init`, `update`, `hitDetect`, `render`, `free`, `getObjectTypeId`, `getExtraSize`.

`classReplaceCallback` patches one slot in place and hands back the original to chain to:

```c
FhClassCallback original = 0;
host->classReplaceCallback(mod, 1 /* camcontrol */, FH_SLOT_UPDATE, my_hook, &original);
```

Every patch is recorded per mod and restored before the library is unloaded, because a slot still
pointing into an unloaded image is a crash on the next dispatch.

This is portable and needs no binary patching, and inlining cannot defeat it because the dispatch is
already indirect. For anything that is not a class method, use symbol hooking.

### Symbol hooking

Any game function can be hooked by name, `static` ones included:

```c
void* target = host->symbolAddress(mod, "mainGetBit");
void* original = NULL;
host->hookInstall(mod, target, my_hook, &original);
```

`original` is a normal function pointer; call it to chain. `hookRemove` restores the function, and
every hook a mod installs is removed automatically before its library is unloaded.

**Game code is compiled with `-fpatchable-function-entry=4`**, so every game function begins with four
`nop` instructions — 16 bytes of scratch. Installing a hook overwrites that pad with an absolute
branch:

```
ldr x16, #8      ; 0x58000050
br  x16          ; 0xd61f0200
.quad replacement
```

The original entry point is then simply `target + 16`, because nothing but padding was overwritten.

That one build flag removes the entire hard part. Instruction relocation is what makes binary hooking
risky and what pulls in a disassembler — Dusklight vendors funchook plus capstone and patches both to
work on Apple arm64. Here there is nothing to relocate, so there is no disassembler, no trampoline
allocator, and no third-party dependency. It costs 16 bytes per function: 132 KB on the binary, 0.67%.

`install_hook` refuses any target whose first four words are not `nop`, so hooking a function that was
not compiled with the pad — anything in Aurora, or libc — fails cleanly instead of corrupting it.

Symbols resolve by walking the executable's own `LC_SYMTAB` at runtime. The binary is not stripped and
carries 21,114 local symbols, so file-local `static` functions resolve by name with no separate symbol
manifest. `dlsym` would only see the 27,641 exported ones.

Two limits are inherent. A call the compiler **inlined** does not go through the symbol and cannot be
intercepted; game code is built at `-O2` with no LTO, so cross-translation-unit calls are real calls
and hookable, while intra-unit calls may not be. And hooks are installed on the game thread at load
time, with no synchronisation against other threads executing the same function.

### Platform support

| | macOS | Linux | Windows |
| --- | --- | --- | --- |
| Asset mods | yes | yes | yes |
| Load a mod library | yes | yes | yes |
| Call game functions | yes | yes | yes |
| Class interposition | yes | yes | yes |
| Hook exported functions | yes | yes | yes |
| Hook `static` functions | yes | yes | **no** |

Verified on macOS/arm64, on Linux for both arm64 and x86-64, and on Windows x86-64 under Wine. macOS
x86-64 is untested; the release build is arm64 only.

The pad is 16 bytes everywhere, so `original = target + 16` is uniform, but the flag that produces it is
not: `-fpatchable-function-entry` counts **instructions** on arm64 and **bytes** on x86-64, so the build
passes `=4` on arm64 and `=16` on x86-64. On x86-64 the branch is `ff 25` (`jmp *(%rip)`) plus an
eight-byte address, and validating the pad means decoding the multi-byte `nop` forms clang emits rather
than comparing against one constant.

Making a code page writable differs by platform. macOS uses `vm_protect` with `VM_PROT_COPY`. Linux uses
`mprotect`, and it must request `PROT_EXEC` alongside `PROT_WRITE`: the hooking code lives in the same
image as the code it patches, so dropping execute permission on that page faults on the very next
instruction. Windows uses `VirtualProtect` with `PAGE_EXECUTE_READWRITE`.

**Statics do not resolve on Windows.** macOS reads `LC_SYMTAB` out of the loaded image and Linux maps
`/proc/self/exe` and reads `.symtab`, so both reach file-local functions. PE keeps no equivalent table in
the executable — `GetProcAddress` sees only the export table, and static functions live in the PDB.
Closing that gap needs a symbol manifest generated at build time, which is what Dusklight's `symgen
manifest` produces.

**Windows also needs a curated export list**, which is not written yet. A mod cannot bind to an
executable without an import library, and exporting everything is not a workaround: a blanket export
collides with the CRT at link time (`_Unwind_Resume` was the first casualty in testing). The export set
has to be filtered down to game symbols, which is what `symgen def` does. `WINDOWS_EXPORT_ALL_SYMBOLS` is
set as a starting point and has not been validated on a real Windows build.

### Not implemented

- A curated Windows export list, and a build-time symbol manifest so statics resolve there
- Link stubs, so mods can build without a copy of the game binary
- Services beyond the nine host functions above — no config, no save, no runtime overlay or texture registration
- ABI version gating in `mod.json`, and any launcher-side compatibility check
- Runtime enable/disable/reload; mods load once at startup

## What the engine already provides

Before designing anything, these already exist and are worth building on.

**A class registry with vtable dispatch.** `gResourceDescriptors[]` (`game/src/main/modelEngine.c`)
is 0x2c1 entries of `ResourceDescriptor*`, and `Resource_Acquire` hands back a handle to an
`ObjectInterface` — a vtable of `init` / `update` / `hitDetect` / `render` / `free` /
`getObjectTypeId` / `getExtraSize`, extended per class. Every object in the game is reached
through it. That gives a mod three operations essentially for free:

| Operation | Mechanism |
| --- | --- |
| Replace a class wholesale | Swap the `gResourceDescriptors[id]` pointer |
| Interpose one method | Copy the `ObjectInterface`, wrap one slot, chain to the original |
| Add a new class | Extend the table and claim an id |

**Story flags in one contiguous, table-described buffer.** `mainGetBit` / `mainSetBits`
(`game/src/main/gameloop_main.c`) index `gGameBitTable[id]` for a bit offset and width, into four
banks of the single buffer `gGameBitSaveData` at fixed offsets 0x24 / 0x564 / 0x5D8 / 0xEF0. Any
mod that wants to read or set story progression — a practice menu, an achievement watcher, a
randomiser — has a clean, complete surface.

**Dynamic state in four contiguous heap regions.** `mmInit` (`game/src/main/mm.c`) carves region 1
(0x380000), region 2 (0x13FF40), region 3 (0x8BFF40) and region 0 (the remaining arena) out of the
OS arena. Everything allocated at runtime lives in those four blocks.

**File I/O chokepoints.** `loadFileByPath` and `fileLoadToBufferOffset`
(`game/src/main/fileio.c`) are where a mod directory could shadow a disc file. Aurora already
handles texture packs.

**A debug overlay and input traces.** `fhDebugOverlayDraw()` is called from `VIWaitForRetrace`
under `FOXHOLLOW_DEBUG_SHORTCUTS` (on by default), and `padUpdate` can record and replay controller
input (`fhInputTraceRecord`/`fhInputTraceReplay`). The substrate for a mod UI and for input playback exists.

**Four camera viewports** with independent scissor rects and viewport transforms
(`gCameraViewports[4]`, `Camera_SetCurrentViewIndex`). Currently used for render passes rather
than split-screen — `newshadows.c` and `engine/0` are the only users — but the plumbing is real.

## Backlog and feasibility

| Mod | Verdict | Effort | Blocked on |
| --- | --- | --- | --- |
| Frame pacing fix | **Done** — was a port bug; fixed | — | — |
| Speedrun practice tools | Very feasible in-session | Low → Medium | Nothing (pacing fix landed) |
| Mirror mode (projection) | ~5 hooks, cannot desync | Low | Nothing |
| Co-op with Tricky as P2 | The only co-op shape this engine can host | Medium | Nothing (local); netcode later |
| CloudRunner ghost racing | Best first networked feature | Medium | Nothing |
| Mirror mode (true world) | Real but a long tail | High | Nothing |
| RetroAchievements | Tractable; process is the risk | Medium | RA's position on source ports |
| Live networked racing | Possible, not lockstep | High | Data-plane design |
| Amethyst as a mod | Needs the ABI to exist first | ABI is the work | Scoping with Rena |

## 1. Frame pacing — fixed in the port

**Done — this turned out to be a port bug, not a mod, and it is now fixed.** Kept here because the
diagnosis matters for the items below: frame-accurate input playback depends on it.

The symptom was real. What caused it was not what this document originally guessed. The full
writeup, with before/after numbers, is in
[Runtime porting and debugging](DEBUGGING.md#retail-counted-what-the-port-measures); the short
version:

`waitNextFrame` (`game/src/main/pi_videoinit.c`) derived its logic timestep by *measuring*
wall-clock time, where retail obtained the same quantity by *counting* VI retraces. The measurement
points are not phase-locked to presentation, so `timeDelta` inherited the variance of whatever game
work sat between them — mean 0.999 but **stdev 0.107**, ranging 0.48 to 1.51. Frames were being
displayed on an exact cadence while the world advanced by a jittering amount of simulated time.

Three theories were wrong and worth recording so they are not re-investigated:

- **Not the presentation clock.** Instrumenting `wait_for_retrace_deadline` in
  `port/src/vi_shim.c` showed `SDL_DelayPrecise` landing within 6 µs of its deadline and the
  resync branch never firing. The software 60 Hz metronome is near-perfect.
- **Not `OSGetTime`.** Logging Aurora's clock against `SDL_GetTicksNS` on identical frames showed
  the two agreeing to within 0.012 ms mean. The `mktime` cost in `OSTime.cpp` is real but was not
  producing the artefact.
- **Not GPU or vsync contention.** `aurora_begin_frame` averaged 0.026 ms, so nothing was blocking
  on the swapchain.

The fix counts presented frames instead of measuring them: `VIGetRetraceCount()` differenced across
calls. `timeDelta` is now exactly 1.0 every frame, and the double-step and clamp paths — 70 and 71
events per 1100 frames respectively — no longer occur at all.

## 2. Speedrun practice tools

**In-session savestates are easy; on-disk savestates are hard.** All dynamic state lives in the four
`mmInit` regions, and restoring a snapshot within the same process works because the addresses are
identical, so the raw pointers inside the snapshot stay valid. Write that same snapshot to disk and
reload it in a later run and every pointer is garbage. On-disk states would need a relocation pass
over the whole heap — a much larger job, and one the pointer-width notes in
[DEBUGGING.md](DEBUGGING.md) suggest would be painful.

**Targeted state save is the practical 90%.** `PlayerState` is 0x8E0 bytes, reached through
`obj->extra`; the camera is `gCameras[]`; story flags are the one `gGameBitSaveData` buffer. A
position-and-state save/restore covers most practice needs without touching the heap at all, and is
what practice mods for other games generally ship.

**A flag editor is a table walk, not reverse engineering.** `gGameBitTable[]` describes every story
bit's offset and width, so "jump to any story state" is a UI problem.

**Frame-perfect input playback is the one hard part**, and it is hard for the reason above: the sim
advanced by an integer `framesThisStep` derived from a wall clock, so a recorded input stream did
not replay against an identical timeline. The pacing fix removes that obstacle: `framesThisStep` is
now deterministically 1 per presented frame, so a recorded stream replays against an identical
timeline.

Effort: Low for save/restore, a flag editor and a timer. Medium-to-high for input playback.

**Conflicts with RetroAchievements hardcore mode**, which forbids savestates and cheats. The two
have to be mutually exclusive at runtime.

## 3. Two-player co-op with Tricky as P2

The instinct to hedge on this one was right, but the conclusion should be the opposite: **Tricky is
the only shape of co-op this engine can host without an architectural rewrite, and that makes it a
good idea rather than a doubtful one.**

Two facts rule out a second Fox:

- **"The player" is a singleton by construction.** `Obj_GetPlayerObject()` is literally
  `objGetAllOfType(0)[0]` (`game/src/main/object.c`) — the first object of type 0 — and there are
  565 call sites. A second player character means disambiguating all of them.
- **The streaming origin is a single global.** `gMapBlockOriginX`/`Z` and `playerMapOffsetX`/`Z`
  are derived from one character position (`game/src/main/shader.c`), and the loaded, collidable,
  visible world is a 16×16 grid of 640-unit blocks around it. Two players more than about seven
  blocks (~4500 units) apart puts one of them outside the loaded set entirely — no terrain, no
  collision, no objects. There is no second window, and the offset is baked into hundreds of render
  and physics sites.

Tricky sidesteps both. He is already a separate object of a different type
(`getTrickyObject()` = `objGetAllOfType(1)[0]`) with his own AI and animation set, and he is already
leashed to Fox — so the shared streaming window stops being a constraint and becomes a description
of existing behaviour. One camera, one HUD, one screen. No split-screen, no second camera
controller, no second viewport.

The work is then bounded: give Tricky's controller a pad source instead of an AI source, inside one
DLL (`game/src/dlls/objects/196_Tricky/tricky.c`). The player's own pad reads are hardcoded to port
0 at roughly eight sites in `player.c`, and stick values are already cached into `PlayerState`, so a
per-character pad index is a small change rather than a sweep.

Open questions, all design rather than engineering:

- Tricky's move set is a companion's — find, dig, flame, stay — not a traversal kit. P2 plays a
  support role. That is a design to embrace, not a limitation to engineer around.
- Cutscenes and scripted sequences drive Tricky directly, so P2 loses control during those and
  needs a clean handoff.
- Tricky is absent for large stretches of the game, so co-op is inherently chapter-scoped.

Effort: Medium. **Local same-machine two-pad co-op first** — that version needs no netcode at all,
and it is the highest fun-per-unit-work item on the list.

## 4. CloudRunner racing

SFA has no bike; the rideables are the CloudRunner (`600_DR_CloudRun`) and the Arwing
(`666_ARWArwing`). Reading "bike race" as "CloudRunner race" is the right call, and the course is
already there.

**A real race controller exists.** `605_CRCloudRace` runs a phase machine — START → RACING → ABORT →
COUNTDOWN → RELOAD → RESET_TO_START — driven by gamebits (`RACE_ACTIVE`, `RACE_STARTED`,
`IN_FINISH_VOLUME`, `CAN_FINISH`), with totem-pole gate objects dispatched through
`objGetNearestTypeTo(...)` → `handleEvent`. Start volume, finish volume, gates and reset-to-start
all work today.

**But it is a solo time trial.** `DR_CloudRunner_getRacePosition()` returns a hardcoded `0`, and
there is no lap counter, rival or on-screen timer in the controller. Multi-racer logic has to be
written, not switched on. The mount layer is reusable though — `setMountState`, `getMountSide`,
`canMount`, `getRiderPosition`, `getCameraPosition` — so a second rider on the same course is a
matter of instantiating another CloudRunner and driving it.

**Start asynchronous.** Ghost racing needs no real-time networking: record the local run, upload it,
download rivals' runs, render them as non-interacting objects. Sizing is trivial — transform samples
at 20 Hz (12 B position, 6 B rotation, 2 B animation id, 2 B frame ≈ 22 B) is ~440 B/s, so a
three-minute run is ~80 KB raw and well under 20 KB compressed. Recording transforms rather than
inputs also sidesteps determinism entirely: a ghost cannot desync because it is never simulated.

Effort: Medium for ghosts, High for live racing.

## 5. RetroAchievements

`rcheevos` (`rc_client`) is the standard integration: it handles login, game identification,
achievement definitions and unlock submission, and asks the host for essentially one thing — a
memory-read callback.

The obstacle is that a source port has no ROM and no emulated memory map. Existing sets for the
GameCube release are written against Dolphin's view of MEM1 at retail DOL addresses; Foxhollow's
globals are native symbols wherever the linker put them, and MEM1 here is a `calloc` region backing
the game's arena.

Two things make this far better than it sounds:

- **The decomp byte-matches**, so `~/Code/sfa/config/GSAE01/symbols.txt` maps every retail address
  to a symbol name that also exists as a native global in this build. A retail → native translation
  table can be generated rather than hand-authored.
- **Most conditions in a story game key off flags, and the flags are one buffer.**
  `gGameBitSaveData` plus `gGameBitTable[]` covers the great majority of a set, with a handful of
  counters for the rest.

Caveats:

- Struct layouts moved wherever pointers widened, so retail → native is not linear *inside* a
  struct. Fine for flag buffers and scalar arrays; needs care for anything read at a raw offset.
- Hardcore mode forbids savestates and cheats, so it is mutually exclusive with the practice tools.
- **The real dependency is process, not code.** RA has to recognise the port as a supported target
  and decide whether existing sets apply or a new set gets authored. That is a conversation to have
  *before* any address-translation work, because the answer determines whether that work is needed
  at all.

Effort: Medium technically; unknown on the process side.

## 6. Amethyst as a mod

The structural point matters more than any detail: **Foxhollow cannot apply a ROM patch.** There is
no DOL to patch and no runtime DLL loading — Phase 4 replaced the disc DLL system with a static
descriptor table. Any existing binary patch has to be re-expressed as source-level changes or
mod-layer hooks; none of it carries over mechanically.

The good news is that the descriptor table described at the top of this document is exactly the
substrate such a mod needs. If the ABI exposes descriptor replacement, method interposition and
asset shadowing, then most of what a ROM patch does — new objects, changed object behaviour,
replaced assets — has a direct native equivalent.

I have not examined Amethyst and should not guess at its internals; Rena is the source of truth for
what it actually needs. The useful thing to bring to that conversation is a concrete ABI proposal
plus the honest constraint about binary patches. Given Rena would likely want to write it,
**the deliverable here is the ABI and its documentation, not the mod.**

## Networking: what the latency budget allows

The simulation runs at 60 Hz, so a frame is 16.7 ms. Anything that adds more than a frame or two to
the input-to-display path is felt.

**Deterministic lockstep is off the table.** It requires bit-identical floating-point results across
machines and platforms. `port/include/foxhollow_compat.h` already documents `__frsqrte` implemented
as exact `1.0/sqrt(x)` where hardware returns a refined estimate, and the port targets macOS/arm64,
Windows/x64 and Linux with different compilers. Cross-platform bit-exactness would be a project in
its own right. Any live multiplayer therefore has to replicate **state**, not inputs.

**Host-authoritative state replication is the right model**, and it is unusually forgiving here:
the second entity is a companion or a rival on a track, not a co-protagonist sharing physics with
the first. Roughly 40 ms of RTT on Tricky's inputs is acceptable, and can be masked by predicting
his movement locally.

### Serverless is right for the control plane and wrong for the data plane

**Control plane — a perfect fit, and exactly the stack you prefer.** API Gateway (HTTP or
WebSocket) + Lambda + DynamoDB for lobby codes, matchmaking, session tokens, presence, and
exchanging NAT-traversal candidates. S3 + DynamoDB for ghost storage and leaderboards, with
presigned upload URLs minted by Lambda. The same stack serves the mod registry and update checks.
Latency is irrelevant — 200 ms to create a lobby is invisible — and it scales to zero, so idle cost
is nil.

**Data plane — keep API Gateway and Lambda out of the frame loop.** The path would be
client → API Gateway → Lambda invoke → `PostToConnection` back through API Gateway → other client.
Even warm, that is tens of milliseconds per hop, with cold starts occasionally adding hundreds —
more added latency than the internet path itself. The cost shape is wrong too: API Gateway
WebSocket bills per message, and two players exchanging state at 60 Hz is roughly 432,000 messages
per player-hour, which becomes real money for a handful of concurrent sessions while delivering
worse latency than a direct socket.

**The right answer is direct peer-to-peer UDP**, with the serverless control plane doing only the
introduction — exchange public endpoints, coordinate the hole punch. Fall back to a relay when
punching fails, which is a persistent UDP process and therefore ECS Fargate, a small Lightsail or
EC2 instance, or GameLift if you want it managed. Lambda cannot hold a UDP socket. The relay is the
only always-on cost, and only for the minority of sessions that need it.

**Make the first networked feature asynchronous.** Ghost racing on the CloudRunner course has no
latency budget at all, uses only the serverless stack, cannot desync, and is a genuinely good
feature. Ship that, learn the backend under zero real-time pressure, and only then decide whether
live co-op justifies a real-time data plane.

## Case study: a mirrored world

Worked through in detail because it touches nearly every subsystem a mod could want — geometry,
collision, streaming, placement, AI routing, input and saves — and so it maps the mod surface more
thoroughly than any of the items above.

### Establish the transform before anything else

The game is Y-up. So `x = -x, y = -y` is `diag(-1, -1, 1)`: determinant **+1**, a 180° roll about
Z. Read as map-plane X/Y — engine X and Z — `diag(-1, 1, -1)` is a 180° yaw, also determinant +1.

Both are *rotations*, and a rigid rotation applied uniformly to everything is unobservable. The
player and the world turn together and the game looks identical. It becomes visible only when
applied non-uniformly, which is a bug rather than a mod: the world rotates, the player does not,
and she is standing on ceilings.

The transform that reads as "inverted" and cannot be reproduced by moving the camera is a
**single-axis mirror**, `x = -x`, determinant **−1**. That is what the rest of this section means
by "mirror". The Y flip is a separate and much worse problem; see below.

### Two implementations, roughly 20× apart in cost

| | Projection mirror | World mirror |
| --- | --- | --- |
| What moves | Nothing; the image is flipped | The world data itself |
| Hook count | ~5 | ~7 chokepoints plus a tail of tens |
| Can desync | No | Yes, everywhere |
| Save compatibility | Automatic | Needs an explicit transform |
| Honest description | Mirror Mode | A modified world |

### A. Projection mirror

`gCameraProjectionMatrix` is the single 4×4 used both by `GXSetProjection` **and** by every
CPU-side world→screen helper — `Camera_ProjectWorldPoint`, `Camera_ProjectWorldSphere` and
`Camera_ProjectWorldPointWithOffset` in `game/src/main/camera.c` each multiply through it by
hand. Negating row 0 once, immediately after `C_MTXPerspective` (four sites in `camera.c`),
mirrors GPU rendering, lens flare, sun occlusion and world-space HUD markers consistently.

Two subsystems need no attention at all:

- **The HUD stays upright.** 2D passes are built in the sibling `C_MTXOrtho` branch, and
  `game/src/track/intersect_render.c` sets its own orthographic projections. A change confined
  to the perspective branch cannot reach them.
- **Culling stays correct.** `mapBlockIsInViewFrustum` (`game/src/main/shader.c`) tests against
  `gViewFrustumPlanes`, built from camera orientation and FOV rather than from the projection
  matrix — and the frustum is X-symmetric regardless.

What remains:

1. **Swap `GXSetCullMode` FRONT↔BACK.** A negative determinant flips winding. There are 76 call
   sites, so this belongs in the GX shim, not in 76 edits.
2. **Negate row 0 of the three `C_MTXLightPerspective` matrices** —
   `gCameraLightPerspective{,Scaled,FlipY}Matrix`, same four sites in `camera.c`. They are texgen
   matrices that sample buffers rendered *with* the mirrored projection, so they have to agree
   with it or projected shadows and the depth-of-field passes land on the wrong side.
3. **Negate stick X** in `padUpdate` (`game/src/main/pad.c`), so that "right" on the stick is
   right on screen.

No game data changes, so collision, AI, triggers, streaming and saves all continue to operate on
the original world and cannot desync. The honest limitation is that this is a *presentation*
mirror: baked-in signage reads backwards, Krystal's staff swaps hands, and nothing in the world
has actually changed.

### B. World mirror

Mirror the data and leave the renderer alone. Seven chokepoints do most of the work.

**1. `mapBlockUnpackFile` — `game/src/main/tex_dolphin.c` (port-owned).** The important one, and the
reason this is tractable at all: **terrain rendering and collision share a single vertex pool.**
`block->vertices` is a packed s16 array (stride 6, `>> 3` fixed point). `setupToRenderMapBlock`
hands it to `GXSetArray(GX_VA_POS, …)`; the collision triangle walker in
`game/src/main/track_dolphin.c` reads the same array. Mirroring it once covers both. Block-local X
spans 0..640, so raw 0..5120, and the mirror is `vp[0] = 5120 - vp[0]`.

Four companion fixes live in the same function, and each one is silent if missed:

- `MapTriIndex.cellMask` — the low byte is an 8-cell X coverage mask. Un-reversed, the broadphase
  rejects every triangle and the map becomes non-solid.
- `MapTriGroup.minX`/`maxX` and `MapBlockBoundsRec.minX`/`maxX` — swap and negate, or bounds
  tests reject their own contents.
- `MapHitLine.x[2]` — the HITS.bin wall segments.
- Swap `vert[1]`/`vert[2]` in each `MapTriIndex`, so collision normals still point out of the
  surface. `game/src/dlls/engine/66/66.c` separates floors from ceilings by `normalY`; get this
  wrong and the world is inside out while looking perfectly normal.

Render winding cannot be corrected inside the baked GX display lists, so terrain still needs cull
mode flipped — but only around `renderMapBlock` and the `setupToRenderMapBlock` calls in
`game/src/main/lightmap_draw.c`, not globally as in approach A.

**2. `mapCoordsToId` and `mapFillCellEntry` — `game/src/main/shader.c`.** World grid cell → block
id. Feeding them `-1 - x`, and mirroring `MapBounds.minX`/`maxX` plus the `MapRomListPage.cells`
row indexing, mirrors the entire 16×16 streaming window and the `playerMapOffsetX` floating origin
for free.

**3. `fhSwapRomListSection` — `port/src/game_endian.c` (port-owned).** Already walks every placement
record in the game. Negating `posX` at +0x08 is one line and covers every placed object at once.

**4. `Obj_RunInitCallback` — `game/src/main/object.c`.** The single place every object class's
`init` runs. Negating `obj->anim.rotY` after it returns fixes initial facing for all 292 classes
without touching a single per-class payload offset. This matters because yaw is *not* in the
placement base header — it lives in the class payload at varying offsets, so there is no equivalent
of hook 3 for rotation.

**5. `RomCurve_add` — `game/src/dlls/engine/20_Hcurves/Hcurves_romcurve.c`.** The single entry point
for every AI waypoint and gated path node. Negate `x` (0x08) and `yaw` (0x2C, s8, 1/256 turns).

**6. `voxLoadVoxMapActual` — `game/src/main/voxmaps.c`.** The per-block traversability bitmap used
for routing. Mirror the X bits, or flying and following NPCs path into walls.

**7. Save load and store.** See the saves section below.

**The tail.** Surveying the per-class placement payloads: only about five carry f32 world positions
(`331_CFMainCryst` start/end X, `400_ECSH_Cup`, `413`, `428_SH_queenear`, `458_DIMExplosio`), and
about 47 carry an `s8 initialYaw`-shaped field, most of which hook 4 already covers. So the tail is
tens of sites rather than hundreds. Cutscene camera splines and scripted sequence points are what
will visibly misbehave and need case-by-case attention.

### The Y flip is a different problem

Up is not a variable in this engine; it is an assumption baked into scalar fields.

- Gravity is a scalar `gravityY` (`game/src/main/model.c`).
- `TrackGroundHit` is `{f32 height; f32 normalX; …}` — `height` is a bare Y, not a signed
  distance along an up vector.
- Floor versus ceiling is `normalY < threshold` (`game/src/dlls/engine/66/66.c`).
- `MapTriGroup.minY`/`maxY`, `block->collisionYOffset` and `block->minY`/`maxY` all assume
  min < max.

On top of that the entire locomotion set — landing, ledge grab, swimming — is authored feet-down.
An upside-down mod is "invert gravity, reclassify every surface, flip every Y bound, re-author
locomotion", and short of all four it mostly falls through the world. Not a starter mod.

### Saves

Saves store **absolute world coordinates**:
`SaveGameCharacterPosition{f32 x, y, z; s8 angle; s8 mapLayer; s8 mapDataFileId}`
(`game/include/main/dll/dll_0017_savegame_api.h`), fed to the player by `objLoadPlayerFromSave`.

- **Under a projection mirror**, nothing needs doing. The world never moved, so saves are fully
  compatible in both directions.
- **Under a world mirror**, the transform is not automatic. A save recorded at `x = 200` in
  vanilla names a location in the *unmirrored* world; loading it into a mirrored world at
  `x = +200` places the player at the mirror image of where they were, frequently inside a cliff
  or over a void. The fix is to treat the save file as un-mirrored world data and apply the
  mirror at the same boundary as everything else: negate `x` and `angle` on load, negate them
  back on save. Saves then round-trip losslessly between vanilla and the mod, which is a property
  worth advertising rather than an implementation detail.
- Either way the streaming origin follows on its own —
  `gMapBlockOriginX = fastFloorf(pos.x / gMapBlockWorldSize)` in `game/src/main/shader.c` is
  recomputed from the loaded position.

The general rule this suggests: **a mod that transforms the world must transform the save file
through the same boundary**, in both directions, or saves stop being portable.

## What the mod layer therefore needs

In rough priority order, derived from what the backlog above actually asks for:

1. **A class ABI over `gResourceDescriptors[]`** — replace a descriptor, interpose a single
   vtable method, register a new class. This is what Amethyst-shaped mods, co-op and racing all
   need, and the engine is already built for it.
2. **Asset shadowing** — `loadFileByPath` and `fileLoadToBufferOffset` in
   `game/src/main/fileio.c` are the chokepoints where a mod directory overrides a disc file by
   name or id. This is what most mods will actually want.
3. **A world-transform descriptor** — the six or seven callbacks from the mirror case study,
   defaulting to identity so a build with no mod loaded is bit-identical to vanilla.
4. **A save-transform pair** bound to the same descriptor, so world-transforming mods keep saves
   portable.
5. **State access for tooling** — read/write gamebits, snapshot and restore the `mmInit` regions,
   and draw into the existing debug overlay. Practice tools, achievements and any future
   randomiser all sit on this.
6. **Load configuration** — `src/main.c` already takes the disc path from argv or
   `FOXHOLLOW_DISC`; a `--mods <dir>` argument plus a per-mod manifest is the natural extension
   and keeps mods out of the shipped binary.

All of it lives in the **port layer, not the game layer**, so it stays out of the periodic decomp
sync.

## Suggested order

1. ~~Fix frame pacing.~~ **Done.** It was a port bug; see item 1 above.
2. **Ship projection Mirror Mode and basic practice tools.** Both are small, both prove the hook
   registry, and both are demonstrable in ten seconds.
3. **Build local Tricky co-op.** Highest fun-per-unit-work, no netcode.
4. **Add CloudRunner ghost racing.** Learn the serverless backend with no latency budget and no
   desync risk.
5. **Then decide** whether the true world mirror, live networked racing, or RetroAchievements
   earns the next block of time — by which point the ABI will have been exercised enough to know
   what it is missing.
