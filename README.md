# Foxhollow

A native port of *Star Fox Adventures* (GameCube) for modern platforms, built on the
[SFA decompilation](https://github.com/zcanann/SFA-Decomp) and
[Aurora](https://github.com/encounter/aurora).

This is a source port, not an emulator: the decompiled game code is compiled natively for the
host CPU, and the Dolphin SDK layer (GX, PAD, DVD, CARD, VI, OS, DSP) is replaced by Aurora and a
thin shim layer. Assets are read straight out of your own disc image at runtime — nothing is
extracted, converted, or redistributed.

> ⚠️ **No game assets are included and none will ever be.** You must provide a disc image of your
> own legally obtained retail copy of the game.

## Status

**Playable end to end.** The game can be completed from boot to credits without a crash, and
nearly everything is a faithful reproduction of retail behaviour.

- **Retail-accurate.** Rendering, gameplay, physics, audio and cutscenes match retail closely
  enough that divergences are now the exception. Where retail itself misbehaves, the port
  reproduces the same glitch for the same reason — see [docs/RETAIL_BUGS.md](docs/RETAIL_BUGS.md)
  for the confirmed cases and the evidence behind them.
- **Real saves, both directions.** Saves live in standard GameCube memory-card storage, so an
  existing *Star Fox Adventures* save from a real card or from Dolphin loads directly, and saves
  the port writes are valid card data that can be restored to genuine hardware.
- **848 of 849 game translation units compile and link** — everything except `main/boot_logo.c`,
  which needs the asset-derived loading-screen textures.
- **Audio is native.** MusyX runs in-process with a software mixer feeding SDL audio; streamed
  music and voice work.

Proven on **macOS (Apple Silicon)**. The shim layer under `port/` contains no platform-specific
code and Aurora supports Windows and Linux, but neither has been built or run yet.

## Requirements

- A disc image of your own retail copy. Currently the **US release (`GSAE01`, revision 0)**;
  see [Remaining work](#remaining-work).
- CMake 3.25+, Ninja, and a C++20 compiler.
- Disc images are read through [nod](https://github.com/encounter/nod), so ISO/GCM, RVZ, WIA and
  the other formats nod supports all work as-is.

## Building

```sh
git clone --recursive git@github.com:JackPriceBurns/foxhollow.git
cd foxhollow
cmake -B build -G Ninja
cmake --build build --target foxhollow
```

The first configure fetches and builds Dawn (WebGPU); expect it to take a while.

CMake options:

| Option                      | Default | Effect                                  |
|-----------------------------|---------|-----------------------------------------|
| `FOXHOLLOW_DEBUG_SHORTCUTS` | `ON`    | Builds the in-game performance overlay. |

## Running

```sh
./build/foxhollow /path/to/starfox-adventures.rvz
# or
FOXHOLLOW_DISC=/path/to/starfox-adventures.rvz ./build/foxhollow
```

Current developer defaults, both of which the launcher will eventually own:

- **Audio starts muted.** Press <kbd>M</kbd> to toggle it.
- Press <kbd>,</kbd> to toggle the performance overlay.

### Saves

Saves are written through Aurora's CARD implementation to Dolphin's memory-card location — by
default the GCI folder for the disc's region, e.g. `<Dolphin user dir>/GC/USA/Card A/`. Each save
is an ordinary `.gci` file, which is the on-card format, so files copy in and out of a real
memory card with any GCI-capable tool and are shared with Dolphin as-is.

### Development and testing tools

| Variable                 | Effect                                                 |
|--------------------------|--------------------------------------------------------|
| `FOXHOLLOW_DISC`         | Disc image path, if not given as `argv[1]`.            |
| `FOXHOLLOW_INPUT_RECORD` | Record every frame's `PADStatus` to a trace file.      |
| `FOXHOLLOW_INPUT_REPLAY` | Replay a recorded trace instead of reading live input. |
| `FOXHOLLOW_DUMP_NEAR`    | Dump the objects near the player each frame.           |

Record/replay is deterministic frame-for-frame and is the basis for the automated regression
testing described below. `docs/DEBUGGING.md` covers the rest of the toolkit: LLDB attach recipes,
runtime render gates, retail disassembly, and live diffing against Dolphin's GDB stub.

## Remaining work

The port itself is largely done; what is left is mostly packaging, coverage, and edition support.

- **Launcher.** A front end for picking the disc image, managing saves, and setting options, so
  running the game does not mean a command line and environment variables.
- **Website.** A project page with setup instructions and releases.
- **Regression testing.** Build out recorded-input traces into an automated suite that replays
  known sequences and checks for divergence, so a fix in one area cannot quietly break another.
- **Any-ROM support.** The build currently pins `VERSION_GSAE01`. Asset lookup should resolve
  against whatever disc is supplied — any region, any revision — rather than assuming one
  release's layout. The disc header already gives the game ID and revision at runtime, so the
  work is in making asset and data mapping version-aware instead of compile-time fixed.
- **Revision 1 (`rev 1`) option.** Support for the changes made in the Player's Choice revision,
  selectable at runtime, so the port has parity with both editions of the game rather than only
  the original release.

## Repository layout

| Path         | Contents                                                                                                                |
|--------------|-------------------------------------------------------------------------------------------------------------------------|
| `game/`      | Vendored decompilation sources (`src/main`, `src/dlls`, `src/track`, `src/musyx`, `include/`), with port fixes applied. |
| `port/`      | The port layer: Dolphin SDK shims, compatibility headers, endian and native-width helpers, and the compiled-unit list.  |
| `src/main.c` | Entry point — Aurora init, disc mount, hand-off to `gameMain`.                                                          |
| `extern/`    | Aurora and Borealis submodules.                                                                                         |
| `tools/`     | Build and analysis scripts.                                                                                             |
| `docs/`      | Porting, debugging, modding and retail-behaviour documentation.                                                         |

Game-code changes are made in this repository; a separate periodic job syncs them back to the
decomp. `game/VENDORED.txt` records the upstream commit.

## Documentation

- [Roadmap](docs/ROADMAP.md)
- [Porting notes](docs/PORTING_NOTES.md) — semantic decisions taken while porting
- [Runtime porting and debugging](docs/DEBUGGING.md) — recurring bug classes, shared endian and
  native-width helpers, LLDB triage, and development tools
- [Retail bug archive](docs/RETAIL_BUGS.md) — confirmed original-game behaviour preserved here
- [Object descriptors](docs/OBJECT_DESCRIPTORS.md)
- [Mod support design notes](docs/MODDING.md) — what a mod layer has to hook, plus a feasibility
  pass over the proposed mod backlog

## Credits

- [SFA-Decomp](https://github.com/zcanann/SFA-Decomp) contributors — the decompilation this port is built from
- [encounter](https://github.com/encounter) — Aurora, Borealis and nod
- [Dusklight](https://twilitrealm.dev/) (TwilitRealm) and [Starship](https://github.com/HarbourMasters/Starship) (Harbour Masters)

## Legal

This project is not affiliated with, endorsed by, or sponsored by Nintendo or Rare. It does not
contain or distribute any original game assets, executables, or copyrighted material. A retail
copy of the game is required to use it.
