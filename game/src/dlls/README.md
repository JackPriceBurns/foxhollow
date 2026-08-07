# DLL source layout

This tree is the planned home for code currently under `src/main/dll/`.
Source files have not been moved yet: the empty leaf directories make the
intended DLL boundaries reviewable before existing translation units are
assigned to them.

Directory names use the SFA resource descriptor's flattened decimal slot:

```text
<bank>/<decimal slot>[_<known name>]/
```

The four banks and their boundaries come from SFA's extracted `DLLS.tab` and
the matching `gResourceDescriptors` array:

- `engine`: slots 0 through 88
- `modgfx`: slots 89 through 170
- `projgfx`: slots 171 through 194
- `objects`: slots 195 through 704

Slot 705 is the resource descriptor table's trailing sentinel, so it does not
have a source directory.

Names are included only when their exact spelling comes from the retail DOL or
the slot's retail `OBJECTS.bin` definitions. Truncated retail names remain
truncated; source-invented names, inferred expansions, and descriptive family
names are not used. Capitalization and underscores are retained exactly. A
number-only directory means retail evidence does not establish one name.

`dlls.txt` records the scaffold in a compact `slot = path` form. Regenerate the
empty folders with `python3 tools/regenerate_dll_scaffold.py`; populated folders
are preserved. Once a DLL source is populated, its generated folder and source
filename are immutable source-truth; semantic cleanup belongs inside the file.
Use `python3 tools/regenerate_dll_scaffold.py --audit-ref <ref> --slots <range>`
to report mapping or payload-filename drift against a known-good tree without
changing files. The audit compares `dlls.txt` spelling and tracked directory
spelling independently, so it reports changes without normalizing historical
case differences. The manifest is not consumed by the build.
