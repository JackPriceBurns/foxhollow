# Mirror Mode

Mirrors the world along X. This is a *presentation* mirror: no game data is transformed, so collision,
AI, streaming and triggers all keep running on the original world and saves stay compatible in both
directions.

## Build

```sh
cmake -S . -B build -G Ninja
cmake --build build
```

## How it works

Four hooks and one port switch.

**The projection.** `gCameraProjectionMatrix` is the single 4x4 used by `GXSetProjection` and by every
CPU-side world-to-screen helper, so negating its row 0 mirrors GPU rendering, lens flare, sun occlusion
and world-space markers consistently. The three `C_MTXLightPerspective` matrices are negated with it, or
projected shadows and the depth-of-field passes land on the wrong side.

The mod hooks `Camera_UpdateProjection` and `Camera_RebuildProjectionMatrix` and negates after each
returns. It cannot negate *before* `GXSetProjection`, because the game calls that in the same function
immediately after building the matrix — so the mod re-pushes the mirrored matrix itself.

**Winding.** A negative determinant flips triangle winding, so front and back faces have to swap or the
world renders inside out. `fhGXSetCullSwap` turns that on in the port's GX shim, which owns
`GXSetCullMode` for game code. Hooking Aurora's copy instead is not an option: patching its page kills
Aurora's worker threads (see docs/MODDING.md).

**Input.** `padGetStickX` is negated so that "right" on the stick is right on screen.

**The pause menu.** `pauseMenuDraw` builds its own perspective view for the holo-display, calling
`Camera_RebuildProjectionMatrix` five times, so it inherited the mirror and came out backwards. The mod
suppresses both the mirror and the cull swap for the duration of that call. The stick un-inverts at the
same time, keyed on `gPauseMenuOpenAmount`, so menu navigation matches what is on screen.

The C-menu item ring goes through `hudDrawCMenu` and is left alone; it reads correctly mirrored. The 2D
HUD and dialogue never needed anything, because those passes are built in the `C_MTXOrtho` branch and
never see the perspective projection.

## Limitations

Baked-in signage reads backwards and Krystal's staff swaps hands — inherent to mirroring the image
rather than the world. `mapScreenDrawHud` has not been checked.
