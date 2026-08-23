# Example Texture Pack

The smallest complete Foxhollow mod: a `mod.json` and one replacement texture. No code, no build step.

Run the game with this directory's parent as the mods root:

```sh
FOXHOLLOW_MODS=$PWD/mods ./build/foxhollow /path/to/sfa.iso
```

Fox is repainted on the title screen. Delete the directory to revert.

## Authoring your own

Textures are matched on the hash of the decoded GX texture, so you never have to unpack `TEX0.tab`.

1. Boot with `FOXHOLLOW_TEXTURE_DUMPS=1` and reach the frame you want to change.
2. Every texture the game bound is written to
   `~/Library/Application Support/Foxhollow/texture_dumps` as an editable 32-bit RGBA `.dds`,
   already named the way a replacement has to be named.
3. Edit it, or paint a new image at any resolution.
4. Drop it in `textures/` under the original filename. `.png` and `.dds` are both accepted.

The filename encodes the texture it replaces: `tex1_{width}x{height}_{texhash}[_{tluthash}]_{format}.png`.
Width, height and hashes describe the *original* texture, not your replacement, so a 4x upscale keeps the
original dimensions in its name. `$` in either hash field is a wildcard.

`textures/` is scanned recursively, so organise it however you like.
