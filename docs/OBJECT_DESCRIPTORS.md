# Object descriptor convention

Object DLL IDs 195 through 704 each have one source-defined object descriptor. The descriptor is
the final top-level declaration in its owning translation unit. DLLs 209 and 210 are the one shared
translation unit; its two descriptors are consecutive at the end in DLL-ID order.

Descriptor symbols use one of two forms:

- `g<Name>ObjDescriptor` when the object has an established name.
- `gDll<HEX>ObjDescriptor` when retail evidence does not establish a useful name. `<HEX>` is the
  uppercase DLL ID without a `0x` prefix.

Every symbol therefore starts with `g` and ends with `ObjDescriptor`. Empty retail descriptors use
the same typed form as the others:

```c
ObjectDescriptor gDll244ObjDescriptor = EMPTY_OBJECT_DESCRIPTOR(0xFFFFFFFF);
```

This replaces undersized `u32[2]` and `u32[12]` placeholders while preserving their retail first
word and zero-initialising the remainder of the native-width descriptor.

Every non-`NULL` entry in `gResourceDescriptors` uses `RESOURCE_DESCRIPTOR_REF(symbol)`. The helper
leaves actual `ResourceDescriptor` objects as-is and converts other concrete descriptor layouts
through `void*`, documenting the central registry's shared-prefix view without redundant casts.

## Retail evidence

The central table is retail data, not a port invention. In GSAE01 it is the 0xB08-byte
`gResourceDescriptors` object at `0x802C6300`: 706 32-bit pointers indexed directly by DLL ID.
Object DLLs occupy entries 195 through 704 and entry 705 is the trailing `NULL` sentinel.

The port's 510 object entries and definitions were compared slot-for-slot with `../sfa`:

- 459 descriptor initialisers match after removing comments and whitespace.
- 49 retail raw-array placeholders have the same first word in `EMPTY_OBJECT_DESCRIPTOR(...)`.
- DLL 195 (Player) is retail's sole `NULL` object slot. The port represents that absence with a
  typed, all-zero `gPlayerObjDescriptor` so the one-descriptor-per-DLL invariant is explicit.
- DLL 201 (Baddie) uses the existing ABI-safe `enemy_initFromDescriptor` callback wrapper. It calls
  retail's three-argument `enemy_init(obj, setup, 0)` with the descriptor callback's two-argument
  signature.

Those last two are the only port-specific descriptor payload cases. The central table remains in
`modelEngine.c` because `Resource_Acquire` and `Resource_Release` consume that exact retail layout;
the large source-level `extern` list is decomp scaffolding rather than a recommended application
architecture.
