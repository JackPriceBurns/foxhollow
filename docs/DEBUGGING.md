# Runtime porting and debugging

Foxhollow runs GameCube game code on a little-endian, 64-bit host. Most runtime bugs found so
far have not been bad gameplay logic: the code is acting on the wrong representation of otherwise
valid retail data.

For investigated oddities that are genuine GameCube behavior and intentionally preserved, see the
[retail bug archive](RETAIL_BUGS.md).

Before changing game behaviour, establish two facts about every suspicious value:

1. Did it come directly from the disc, or was it created by native code?
2. Does it contain a pointer, pointer-sized handle, or an offset calculated around a pointer?

Those questions identify most of the recurring bug classes below.

## Recurring bug classes

| Symptom | Common cause | First check |
| --- | --- | --- |
| IDs, game bits, counts, or distances are wildly wrong | A big-endian multi-byte field was read as native-endian | Inspect the original bytes and use the `fhReadBE*` helpers |
| An object loaded from the map works, but a sequence-created copy does not | The same placement struct can contain raw disc data or native synthesized data | Check `OBJANIM_FLAG_OWNS_PLACEMENT_DATA` and use `ObjAnim_ReadPlacementS16` |
| A model, camera, UI, or object state breaks after an apparently unrelated field | A pointer widened from 4 to 8 bytes and moved every following field | Compare native `offsetof` values and replace hardcoded retail offsets with named fields |
| A cache or table corrupts a nearby allocation | A pointer array was allocated with `count * 4` | Allocate with `count * sizeof(*table)` |
| A function behaves correctly until it returns | An 8-byte out-parameter was written through a cast to a 4-byte local | Audit the callee's real output type; never cast `u32*` or `int*` to `uintptr_t*` |
| A pointer passed through an event or message becomes invalid | The pointer was stored in a `u32` parameter | Carry it as `uintptr_t` and use the native-width message API |
| Colours have swapped channels or a full-screen tint | A packed scalar was reinterpreted as a byte struct on a different-endian host | Construct the byte struct explicitly |
| A few animated vertices explode only at extreme poses | A retail saturating conversion was replaced by a normal C cast | Match the original PPC conversion semantics |

### Big-endian file data

Disc-backed placement data, asset headers, animation streams, tab files, and many embedded tables
remain big-endian until a consumer converts them. Byte fields often continue to work, making a
broken structure look mostly correct.

Do not infer that a whole buffer is native-endian because one part of it was unpacked. Follow the
ownership of each pointed-to subtable. Object definitions were a real example: converting the
header did not automatically convert tables reached through offsets in that header.

For raw 16-bit fields, use the shared helpers described below instead of repeating casts and
byte swaps. Besides being clearer, the helpers work for unaligned fields and do not dereference
raw storage through an incompatible pointer type.

### Mixed raw and native placement data

`ObjAnimComponent::placementData` does not have one universal byte order:

- Map/romlist placement data is still big-endian.
- Placement data allocated or owned by a native object is already host-endian and is marked with
  `OBJANIM_FLAG_OWNS_PLACEMENT_DATA`.

Blindly swapping both cases fixes one path and breaks the other. Use
`ObjAnim_ReadPlacementS16(&obj->anim, &placement->field)` for signed 16-bit placement fields whose
provenance can vary. Use `ObjAnim_ReadPackedS16` only for data that is always a packed big-endian
stream.

### Pointer width affects more than casts

GameCube pointers are four bytes; native pointers are eight bytes. The consequences fall into
three related groups.

First, pointer-bearing structures grow. Every member after the first pointer moves, so code that
uses a retail byte offset may read the middle of a pointer or a different field. Prefer typed
members. If an exact binary overlay really is required, unpack the retail layout into a separate
native structure rather than making one structure serve both layouts.

Second, allocation sizes must follow the native element type. The object-definition cache used
`gObjFileCount * 4` for an array of pointers. On the port, the second half of the cache overlapped
the following reference-count allocation. Loading high-numbered object definitions wrote pointer
bytes into low-numbered refcounts; object 148 then appeared cached even though its cache entry was
null. The resulting crash occurred much later when a sequence tried to use the missing object.

Use this form for every typed allocation:

```c
table = mmAlloc(count * sizeof(*table), tag, flags);
```

Third, output parameters write the size promised by their type. `ObjMsg_Pop` once wrote an
eight-byte sender through a caller's casted four-byte `int`. That overwrote an adjacent saved
register in `collectible_update`, so the key-pickup logic completed and the function crashed only
while returning. A cast can silence the compiler, but it cannot make the destination larger.

### Packed scalars are not byte structures

Code such as the following depends on host byte order:

```c
u32 packed = 0x666666ff;
GXColor color = *(GXColor *)&packed;
```

On GameCube that value is gray RGBA. On a little-endian host the byte order changes, which caused
the red cast over the Ship Battle intro. Construct `GXColor` from explicit channels, or unpack a
scalar with shifts when the scalar representation itself is part of the format.

### Retail intrinsics have semantics beyond their result type

Paired-single stores and the `OSFastCast` family saturate out-of-range values. A plain C cast may
wrap or invoke implementation-defined behaviour instead. This caused a small set of Ship Head
vertices to cross the model during an animation: retail clamped the value to `32767`, while the
port wrapped it to a large negative value.

When replacing a platform intrinsic, treat its rounding, saturation, NaN, and out-of-range
behaviour as part of the API contract.

### Decomp-faithful storage can still be wrong on the port

The decomp describes the retail ABI. Fixed byte counts, opaque stack buffers, global overlays, and
pointer-through-integer fields may be faithful to that ABI while being unsafe natively. Examples
encountered during the Galleon work include:

- an undersized skeleton-hit scratch array overflowing into neighbouring stack state;
- hardcoded structure offsets reading shifted native fields;
- pointer-bearing messages and tables retaining their retail `u32` storage;
- fixed allocations assuming every table element is four bytes.

If retail collision or movement is known to be solid, investigate the data, object state, and
layout before modifying collision response or animation constants. A gameplay workaround often
hides the corrupted input and creates a second bug.

### First-use geometry gaps and Aurora's pipeline caches

If map geometry disappears briefly the first time an area or material is rendered, then remains
correct even after restarting Foxhollow, check GPU pipeline compilation before investigating disc
streaming. Aurora compiles newly encountered GX pipeline configurations asynchronously. A draw is
skipped while its pipeline is unavailable, then succeeds once compilation finishes.

Aurora persists two caches under the platform application-support directory:

- `pipeline_cache.db` is the transferable cache. It contains pipeline hashes, versions, first-use
  ordering, and GX rendering-state configurations such as vertex formats, TEV stages, fog,
  lighting, alpha, blend, depth, culling, and MSAA state.
- `dawn_cache.db` is Dawn's device and driver cache. It is opaque and may depend on the GPU, OS,
  and driver, so it should not be treated as a distributable cache.

The transferable pipeline cache contains no textures, palettes, models, vertices, maps,
animations, audio, scripts, ROM bytes, or file paths. GameCube software supplies fixed-function GX
state rather than programmable shaders; Aurora generates its own host shaders from that state.

Aurora can merge a bundled `initial_pipeline_cache.db` into each user's persistent cache. A
representative release seed can be assembled from thoroughly exercised playthrough caches, but it
only covers configurations that were actually encountered. To prevent first-run draw gaps on a
new machine, pair the seed with a prewarm stage that pumps the window and waits for
`aurora_get_stats()->queuedPipelines` to reach zero. Alternatively, make first-use GX pipeline
creation blocking; that trades missing geometry for a possible compilation hitch.

## Shared endian and native-width helpers

The game target force-includes `port/include/foxhollow_compat.h`, where the global endian helpers
live. Port shim translation units should include that header explicitly when they use them.

### `fhReadBE16`

```c
u16 fhReadBE16(const void *ptr);
```

Reads an unsigned 16-bit big-endian value from raw bytes. Use it for counts, unsigned IDs, masks,
dimensions, and unsigned fixed-point fields.

```c
u16 textureId = fhReadBE16(raw + textureIdOffset);
```

### `fhReadBES16`

```c
s16 fhReadBES16(const void *ptr);
```

Reads the same two-byte representation as a signed value without relying on an extra unsigned-to-
signed cast. Use it for signed offsets, rotations, game bits, coordinates, and signed animation
samples.

```c
s16 gameBit = fhReadBES16(raw + gameBitOffset);
```

### `ObjAnim_ReadPackedS16` and `ObjAnim_ReadPlacementS16`

These helpers live in `game/include/main/objanim_internal.h`:

```c
s16 ObjAnim_ReadPackedS16(const void *value);
s16 ObjAnim_ReadPlacementS16(const ObjAnimComponent *objAnim, const void *value);
```

Use `ObjAnim_ReadPackedS16` when the source is always a big-endian packed stream. Use
`ObjAnim_ReadPlacementS16` when a placement may be raw romlist data or a host-owned native copy.

```c
s16 collectBit =
    ObjAnim_ReadPlacementS16(&obj->anim, &placement->collectGameBit);
```

Do not use the placement helper for arbitrary object state merely because the field is an `s16`;
its ownership decision is specifically about `placementData`.

### `ObjMsg_PopNative`

Object-message sender and parameter storage is now `uintptr_t`. The compatibility `ObjMsg_Pop`
still exposes a 32-bit parameter for callers whose values are genuinely scalar `u32`s.
`ObjMsg_PopNative` is required when either value may contain a pointer:

```c
u32 message;
uintptr_t senderValue;
uintptr_t param;

while (ObjMsg_PopNative(obj, &message, &senderValue, &param) != 0) {
    GameObject *sender = (GameObject *)senderValue;
    /* Use sender and param. */
}
```

Never pass `(uintptr_t *)&someInt`, `(u32 *)&somePointer`, or similar casted output storage. If an
output is unused, pass `NULL`; this also avoids unnecessary writes and made the collectible pickup
fix simpler.

## Development shortcuts

Development-only behaviour is controlled by the CMake option `FOXHOLLOW_DEBUG_SHORTCUTS`. It is
currently enabled by default for the port-shim target.

Configure a normal debugging build with shortcuts enabled:

```sh
cmake -S . -B build -G Ninja -DFOXHOLLOW_DEBUG_SHORTCUTS=ON
cmake --build build --target foxhollow
```

Configure a build with the audio toggle disabled:

```sh
cmake -S . -B build -G Ninja -DFOXHOLLOW_DEBUG_SHORTCUTS=OFF
cmake --build build --target foxhollow
```

CMake caches this value in the build directory, so check the configure command rather than
assuming the default is active.

### Audio mute toggle

The same option enables the `M` key audio toggle in `port/src/ai_shim.c`. It changes the gain on
both the MusyX DMA stream and streamed audio, so movie/dialogue audio and normal music/effects are
muted together. The terminal logs `[foxhollow] audio muted` or `unmuted` on each edge-triggered
toggle.


## Crash investigation checklist

1. Capture the first faulting access and full backtrace under LLDB.
2. Inspect the bad pointer or value in hexadecimal; mixed high/low halves often expose an overlap
   or partial-width write.
3. Determine whether the value came from raw disc data, native state, or a mixed-provenance
   placement.
4. Check every pointer-bearing structure and allocation on the path with native `sizeof` and
   `offsetof`.
5. Audit out-parameters for casts that hide a width mismatch.
6. Compare the raw asset bytes or retail disassembly before changing gameplay logic.
7. Remove temporary traps and logging, build, run `git diff --check`, and reproduce through the
   normal path when a shortcut could affect the result.
