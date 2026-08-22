# Changelog

Entries are keyed by git tag. Tagging a commit publishes the matching section here as the
GitHub release body and as the release notes shown in the launcher, so the heading must match
the tag exactly.

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
