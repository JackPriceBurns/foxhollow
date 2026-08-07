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

## References
- `extern/aurora` — the compatibility layer (see docs/ and examples/simple.c)
- `extern/borealis` — cross-platform port modules (logging, crash, data dirs, updates)
- TwilitRealm/dusklight — Twilight Princess port, same architecture; the working example to
  crib from (vendored decomp + `src/dusk` port layer + Aurora/Borealis submodules)
- HarbourMasters/Starship — Star Fox 64 port (N64 lineage, sibling game)
- docs/ROADMAP.md — the phase plan
