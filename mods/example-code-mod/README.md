# Example Code Mod

A proof-of-concept native mod. It calls into game code and interposes an object class callback to
force the camera's field of view to 90°, which is visible from the first rendered frame.

## Build

```sh
cmake -S . -B build -G Ninja
cmake --build build
```

The library is written to `lib/<platform>-<arch>/mod.so` inside this directory, which is where the
loader looks for it. Nothing else has to be installed; the game finds it on the next launch.

On macOS the mod links against the game binary with `-bundle_loader`, which is what lets it call
game functions. Point `FOXHOLLOW_BINARY` at a different build if yours is not at
`../../build/foxhollow`.

## What it demonstrates

- **Lifecycle.** `fh_mod_initialize`, `fh_mod_update` (every frame) and `fh_mod_shutdown`.
- **Host services.** `modId`, `classCount`, `frameCount` and `log`, through the `FhModHost` table
  passed to `fh_mod_initialize`.
- **Calling game code.** `Camera_GetFovY`, `Camera_SetFovY` and `Obj_GetPlayerObject` are resolved
  straight out of the game binary. No shim, no wrapper.
- **Interposition.** `classReplaceCallback` swaps the camcontrol class's `update` slot for the mod's
  own function, which chains to the original and then overrides the FOV.

Expected output:

```
foxhollow: mods: [dev.foxhollow.example-code-mod] info: hello from dev.foxhollow.example-code-mod (abi 1, 705 classes)
foxhollow: mods: [dev.foxhollow.example-code-mod] info: called into game code: Camera_GetFovY() = 0.00, ...
foxhollow: mods: dev.foxhollow.example-code-mod: code loaded from lib/macos-arm64
foxhollow: mods: [dev.foxhollow.example-code-mod] info: interposed Camera_update; vanilla fovY 60.00 -> 90.00
```

## Status

This is a proof of concept, not the shipping ABI. See `docs/MODDING.md` for what is still missing.
