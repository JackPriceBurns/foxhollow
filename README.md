# Foxhollow

A native port of *Star Fox Adventures* (GameCube) for modern platforms, built on the
[SFA decompilation](https://github.com/zcanann/SFA-Decomp) and
[Aurora](https://github.com/encounter/aurora).

**Status: pre-alpha scaffold.** The app boots an Aurora window; no game code is wired in yet.
See [docs/ROADMAP.md](docs/ROADMAP.md) for the plan.

> ⚠️ **No game assets are included and none will ever be.** You must provide a disc dump of
> your own legally obtained retail copy of the game.

## Building

Prerequisites: CMake 3.25+, Ninja, a C++20 compiler (clang, GCC, or MSVC).

```sh
git clone --recursive git@github.com:JackPriceBurns/foxhollow.git
cd foxhollow
cmake -B build -G Ninja
cmake --build build --target foxhollow
```

The first configure fetches and builds Dawn (WebGPU); expect it to take a while.

## Documentation

- [Roadmap](docs/ROADMAP.md)
- [Porting notes](docs/PORTING_NOTES.md)
- [Runtime porting and debugging](docs/DEBUGGING.md) — recurring bug classes, shared endian and
  native-width helpers, LLDB triage, and development tools

## Credits

- [SFA-Decomp](https://github.com/zcanann/SFA-Decomp) contributors — the decompilation this port is built from
- [encounter](https://github.com/encounter) — Aurora and Borealis
- [Dusklight](https://twilitrealm.dev/) (TwilitRealm) and [Starship](https://github.com/HarbourMasters/Starship)
  (Harbour Masters) — the projects that proved the pattern

## Legal

This project is not affiliated with, endorsed by, or sponsored by Nintendo or Rare. It does not
contain or distribute any original game assets, executables, or copyrighted material. A retail
copy of the game is required to use it.
