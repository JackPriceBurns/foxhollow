# Foxhollow

Foxhollow is a fully native port of **Star Fox Adventures** for modern platforms, built on the
[SFA decompilation](https://github.com/zcanann/SFA-Decomp) and
[Aurora](https://github.com/encounter/aurora).

This port attempts to stay as true to the original game as possible (bugs included). The port is built using Aurora
which gives us support for Mac, Windows, Linux, iOS and Android. Foxhollow has been tested and is fully playable for
Mac, Windows and Linux. There are currently no plans to release for iOS or Android.

You will need a genuine, legally obtained copy of Star Fox Adventures to run Foxhollow (any copy is fine). Foxhollow
does not distribute any original game assets.

## How to run

If you're looking to play Foxhollow, please go to https://foxhollow.dev and download the official launcher to play. The
launcher keeps your game up to date, allows you to manage your saves and also offers a mod library. The instructions
below are for people who are improving contributing to the port or building mods.

### Prerequisites

- CMake 3.25+
- Ninja
- C++20 compiler
- Check .github/workflows/publish.yml for platform specific prerequisites

### Commands

Clone the repo:
```sh
# SSH
git clone --recursive git@github.com:JackPriceBurns/foxhollow.git

# HTTPS
git clone --recursive https://github.com/JackPriceBurns/foxhollow.git
```

Build:
```sh
cmake -S . -B build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DFOXHOLLOW_DEBUG_SHORTCUTS=ON
cmake --build build-debug --target foxhollow
```

Start:
```sh
./build/foxhollow path/to/disc.iso
```

When building and launching Foxhollow in debug mode, there is no sound by default and some debug info on the screen. You
can toggle sound by pressing `m`. 

## Modding

Please check out https://foxhollow.dev/modding for a guide on how to build some simple mods. There is currently no easy
way to introduce whole new game objects, models and animations, there's an open opportunity here for someone interested
to build that functionality.

With modding you are able to fairly easily switch out assets from the disc, similar to how Dolphin texture packs work.
You can also patch pretty much every function in the game so there should be a lot of freedom to be able to build what
you like.

## Contributing

Anyone is allowed to contribute but make sure everything is thoroughly tested. Create a PR into main with your change
and provide a descriptive PR description. AI is allowed, but any changes must be thoroughly tested and AI must not be
used for writing PR descriptions, documentation, commit messages, code comments or anything that should be read by
another human.

## Credits

- [SFA-Decomp](https://github.com/zcanann/SFA-Decomp) the decompilation this port is built from
- [encounter](https://github.com/encounter) Aurora, Borealis and nod
- [Dusklight](https://twilitrealm.dev/) (TwilitRealm) and [Starship](https://github.com/HarbourMasters/Starship)
  (Harbour Masters)

## Legal

This project is not affiliated with, endorsed by, or sponsored by Nintendo or Rare. It does not contain or distribute
any original game assets, executables, or copyrighted material. A retail copy of the game is required to use it.
