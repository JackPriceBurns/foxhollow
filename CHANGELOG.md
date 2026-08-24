# Changelog

Entries are keyed by git tag. Tagging a commit publishes the matching section here as the
GitHub release body and as the release notes shown in the launcher, so the heading must match
the tag exactly.

## v0.4.1 — 2026-08-24

### Fixed
- Native mods work on Windows. The export list the mod import library is generated from was missing
  most of what a mod needs: it matched only `T`, `D` and `B` symbols, so the `-fcommon` common symbols
  that most game globals are — `gCameraProjectionMatrix` among them — and the read-only tables were
  dropped, 9,343 of game.lib's 12,853. It also covered the game library alone, so a mod could call
  game code but not Aurora's GX API or the port's own shims. It is now generated from the game, the
  port shims and the Dolphin-SDK libraries, across all five symbol kinds, which is the surface macOS
  already had through `-bundle_loader`.
- Mirror Mode runs on Windows. Its host declarations now use `FH_MOD_IMPORT`, which is
  `__declspec(dllimport)` on Windows and nothing elsewhere; without it, imported data does not resolve
  at link time.

### Changed
- The example mods default to a `Release` build and pin the release CRT. Built with no build type they
  linked the debug CRT and depended on `ucrtbased.dll` and `VCRUNTIME140D.dll`, which ship only with
  Visual Studio, so the mod loaded on the machine that built it and nowhere else.

### Removed
- The example texture pack and `tools/make_example_texture.py`.

## v0.4.0 — 2026-08-24

### Added
- Mod support. A mod is a directory with a `mod.json` under the folder named by `--mods` or
  `FOXHOLLOW_MODS`, or `mods` beside the save data. Textures and whole disc files can be replaced
  with no code at all, and native mods can call into game code, replace an object class's callbacks,
  or hook game functions by name.
- Three example mods: a texture pack, a mod that interposes the camera, and Mirror Mode, which
  mirrors the world without transforming any game data, so saves stay compatible in both directions.
- Mods declare the mod ABI and the range of Foxhollow versions they support, and the loader refuses
  anything that does not match rather than failing later in a way that is hard to diagnose.
- `FOXHOLLOW_TEXTURE_DUMPS` writes every texture the game binds to the cache directory, named the way
  a replacement has to be named, which is how texture packs are authored.

### Changed
- `TARGET_PC` conditionals are gone from game code. The port is the only target, so the dead
  GameCube branches have been removed.

## v0.3.2 — 2026-08-23

### Added
- The shader cache directory is taken from `FOXHOLLOW_SHADER_CACHE`, so the launcher can hand over
  a shared pipeline cache to start from. Areas other players have already been through compile far
  fewer shaders on the first run.

## v0.3.1 — 2026-08-23

### Fixed
- The Linux build no longer refuses to start with `libjpeg.so.8: cannot open shared object file`.
  It was linked against the JPEG library under the name Debian and Ubuntu give it, which no other
  distribution ships, so it only ran on the one it was built on. It is now built into the binary.

## v0.3.0 — 2026-08-23

### Added
- Linux builds, published from CI alongside macOS and Windows.

### Fixed
- The frame rate no longer halves on 60Hz displays. Presentation waited for the display to finish
  scanning out, which stopped the renderer recycling its frame buffers, so each frame cost two
  refreshes instead of one. Also affects Windows.
- Two globals were sized for the retail data layout rather than the native one and wrote past their
  own storage into whatever the linker placed next. The voxel map manager corrupted the object type
  list, crashing on load, and the looped sound table overwrote the streamed audio table, silencing
  all music. Both had been latent on every platform.
- Cloudrunner path values in Dragon Rock are read at their full width again, rather than losing a
  byte to the value stored after them.

## v0.2.0 — 2026-08-23

### Added
- Windows fully playable end-to-end!

## v0.1.1 — 2026-08-22

### Added
- Windows builds, published from CI alongside macOS.

### Fixed
- Streamed music and dialogue are no longer silent. The disc path prefix was read by walking off
  the end of a neighbouring global, so every stream failed to open.
- Audio no longer crashes on startup. The stream buffer free list was built over whichever globals
  followed the transfer queue rather than over the buffer array itself.

## v0.1.0 — 2026-08-21

### Added
- Native port of Star Fox Adventures running on Aurora.
- Support for every retail disc: USA 1.0 & 1.1, PAL 1.0 & 1.1, and Japan 1.0.
- An "Exit To Desktop" option on the save select screen.
