# Changelog

Entries are keyed by git tag. Tagging a commit publishes the matching section here as the
GitHub release body and as the release notes shown in the launcher, so the heading must match
the tag exactly.

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
