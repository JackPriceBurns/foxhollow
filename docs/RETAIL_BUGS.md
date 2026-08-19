# Retail bug archive

This document records surprising behavior that has been confirmed in the original GameCube game.
These cases are preserved in Foxhollow unless a project decision is made to offer optional fixes.
The purpose of each entry is to retain the evidence and explain why the behavior occurs, rather
than merely catalogue visible glitches.

## SharpClaw disguise ledge-grab T-pose

### Status

Confirmed retail behavior. Intentionally not fixed.

### Symptoms

Fox can enter a bind-pose-like T-pose during the initial ledge grab while wearing the SharpClaw
disguise. The ledge interaction and player state continue to work; only the animation is wrong.

The behavior has three unusual conditions:

- It affects the SharpClaw disguise but not normal Fox.
- It occurs when reaching the area while travelling from Thorntail Hollow towards Cape Claw.
- Travelling through the same area from Cape Claw towards Thorntail Hollow can play the correct
  disguise animation.

### Player move resolution

The ledge state in `playerStateGrabLedge` selects one of two entry moves according to which end of
the detected ledge is nearer. The two player models map those moves to different animation assets:

| Player model                      | Ledge moves      | Animation assets | Source                  |
|-----------------------------------|------------------|------------------|-------------------------|
| Normal Fox, model `0x001`         | `0x00D`, `0x022` | `0x107`, `0x108` | `PREANIM`               |
| SharpClaw disguise, model `0x4E9` | `0x00D`, `0x022` | `0x105`, `0x106` | Current map `ANIM` bank |

The mappings come from the retail `MODANIM.TAB` and `MODANIM.BIN` data. The currently observed bad
state requested move `0x00D`, which the disguise correctly resolved to animation `0x105`. This was
not a missing-animation fallback or a corrupt animation identifier.

### Why normal Fox works

`animLoadFromTable` checks `PREANIM.TAB` before consulting the merged map animation table. Normal
Fox's assets `0x107` and `0x108` are marked as present in `PREANIM`, so they are loaded from the
always-available player animation bank regardless of which map resources are resident.

The disguise assets `0x105` and `0x106` are not present in `PREANIM`. They therefore resolve
through the current map's `ANIM.TAB` and `ANIM.BIN` files.

### Map animation banks

The game cannot keep every animation in memory. `PREANIM` holds common animations that must remain
available, while each map directory supplies an `ANIM` bank containing the assets needed in that
area. Two map banks can be resident simultaneously so an adjacent area can be streamed without a
loading screen. `mergeTableFiles` combines their tables and gives marked full entries priority
over unmarked fallback entries.

All map banks use the same animation ID namespace. Lightweight transition maps retain complete
tables but replace animations they do not expect to use with tiny one-frame static placeholders.
This avoids missing table entries while keeping those banks small.

The relevant retail data differs as follows:

| Map bank    | Animation `0x105`            | Animation `0x106`            |
|-------------|------------------------------|------------------------------|
| `swaphol`   | Full ledge animation         | Full ledge animation         |
| `capeclaw`  | Full ledge animation         | Full ledge animation         |
| `lightfoot` | Full ledge animation         | Full ledge animation         |
| `linkh`     | One-frame static placeholder | One-frame static placeholder |

The `linkh` placeholders contain no animated joint changes. Applying one to the disguise leaves
the model in its default pose, producing the visible T-pose.

### Why direction matters

The bad route was captured live with the following state:

- Current map event ID: `72`
- Resource-directory remap: `70`, or `linkh`
- Primary `ANIM` slot owner: none
- Secondary `ANIM` slot owner: `linkh`
- Merged table entry for animation `0x105`: the `linkh` placeholder
- Saved `mapDataFileId`: `70`, or `linkh`

At that point the full Thorntail Hollow-side animation bank had been unloaded and the Cape Claw-
side bank was not supplying an override. The only available definition for disguise animation
`0x105` was therefore the transition map's static placeholder.

On the working reverse route, a full area bank remains resident while the transition bank is in
use. Its marked definitions of `0x105` and `0x106` outrank the unmarked `linkh` placeholders during
the merge, so the same disguise move IDs resolve to the real animations. The working route's exact
pair of live slot owners was not captured, but this outcome follows from the observed retail
behavior, the two-slot loader, and the retail tables: every relevant full bank supplies the real
assets, while `linkh` supplies the placeholders.

The direction dependency is therefore not part of the ledge detector. It is a consequence of
which animation banks survive the asymmetric map streaming and unloading sequence.

### Scope of the placeholder problem

An exhaustive resolution of the disguise model's 858 animation-table entries against the live
`linkh` bank found 305 moves that select one-frame placeholders but select full animations from
the Cape Claw bank.

| Move group | Placeholder moves in `linkh` | Normal relevance                               |
|------------|-----------------------------:|------------------------------------------------|
| `0x0xx`    |                            2 | The two ledge-entry moves, `0x00D` and `0x022` |
| `0x1xx`    |                            0 | None                                           |
| `0x2xx`    |                          125 | High-numbered special or script-selected moves |
| `0x3xx`    |                          118 | High-numbered special or script-selected moves |
| `0x4xx`    |                            0 | None                                           |
| `0x5xx`    |                           60 | High-numbered special or script-selected moves |

For ordinary player control, the two ledge-entry variants are the only affected moves. The other
303 entries are latent: they would display the same default pose if an interaction or sequence
explicitly requested them while only the `linkh` bank was available, but the ordinary disguise
state machine does not select them in this area.

### Evidence

The explanation was established from three independent sources:

1. LLDB inspection of the active player model, requested move, resolved animation ID, animation
   frame data, merged table, map resource owners, and saved map-data directory.
2. Direct comparison of the retail `MODANIM`, `PREANIM`, and per-map `ANIM` tables and payloads.
3. PowerPC disassembly of the byte-matching decomp objects for animation source selection and
   two-bank table merging.

Relevant decompiled paths are:

- `game/src/dlls/objects/195_Player/player.c`: `playerStateGrabLedge`
- `game/src/main/model.c`: `animLoadFromTable`, `loadAnimation`
- `game/src/main/pi_dolphin.c`: `mergeTableFiles`, map resource loading and unloading

### Conclusion

The banking system is intentional: it is how the game fits a large animation library into the
GameCube's memory while streaming connected areas. The bug is the interaction between three
otherwise reasonable content decisions:

1. Normal Fox's ledge animations were made permanently resident.
2. The disguise equivalents were left map-local.
3. The `linkh` transition bank uses placeholders for those disguise animations even though the
   player can enter it while disguised.

The port reproduces the result because it is reading and applying the retail tables correctly.
