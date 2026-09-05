# Retail test contracts

These expectations were checked against the GSAE01 PowerPC assembly in the local SFA reference
checkout, `~/Code/sfa/build/GSAE01/asm/`. Paths below are relative to that directory; addresses are
retail virtual addresses. They are evidence references, not addresses used by the native tests.

All fixtures are synthetic. Positions, health, callback results, message payloads, and animation
offsets are chosen to distinguish behavior. They are not captured game state or extracted assets.
The tests assert the relationships and state changes demonstrated by retail instructions. No
full-game replay or fresh Dolphin comparison was performed for this suite.

## Portability regressions

| Test | Retail reference | Contract checked |
|------|------------------|------------------|
| `pressure_switch` | `dlls/objects/251/251.s`, `PressureSwitchFB_animEventCallback`, `0x8017AC68–0x8017AD38` | Capture each of ten object references' X/Z positions; reset clears all ten references, restores saved Y and placement Z, clears the game bit and animation command. |
| `grimble_path` | `dlls/objects/208_Grimble/Grimble.s`, `grimble_attachNearestPath`, `0x801627F4–0x80162A7C` | Iterate group `0x17` as object pointers; choose the smaller returned distance; retain that path, progress, sample and rotation. Retail callback byte offsets `0x20/0x24/0x30/0x34` are native slots `8/9/12/13`. Y delta is sample Y minus object Y. The fixture's forward direction and random result 10 produce target progress 2 from progress 3. |
| `sequence_messages` | `dlls/objects/323_FEseqobject/FEseqobject.s`, `FEseqobject_SeqFn`, `0x801DF694–0x801DF80C` | Incoming `0xF000B/C/D` become `0x130001/2/3`; search group 3 for definition `0xF7` and send with the sequence object as sender and zero parameter. |
| `effect_cleanup` | `dlls/objects/279_AppleOnTree/AppleOnTree.s`, `AppleOnTree_free`, `0x8017DC6C–0x8017DC88` | Forward the owner pointer unchanged to effect interface slot 5. Retail leaves the incoming pointer in `r3` for the call. |
| `deferred_messages` | `dlls/objects/325_CloudPrison/CloudPrison.s`, `CloudPrisonControl_update`, `0x8019ADA0–0x8019ADD0`; queue allocation at `0x8019AE14–0x8019AE18` | Store each message ID, sender pointer and parameter in a distinct record, preserving order. Retail records are 12 bytes and the queue holds ten messages; native pointer growth must not make records overlap. |
| `enemy_path_cleanup` | `dlls/engine/25/25.s`, `dll_19_releaseState`, `0x8011205C–0x80112078` | Release the embedded route work; if the path pointer is nonnull, free that allocation and clear it. A second cleanup skips the cleared path. |
| `thornbush_hits` | `dlls/objects/590/590.s`, `drakord_thornbush_hitDetect`, `0x8020BC24–0x8020BC94` | Remember the attacker pointer, subtract damage only for a new attacker contact, and clear that remembered pointer when contact ends. |

The high-address allocations deliberately expose 32-bit pointer truncation. Native pointer values
and structure sizes need not equal retail; retaining the same object identity and behavior does.
All seven tests were also built against the pre-fix sources: each fails there and passes with the
native portability corrections, including optimized builds.

## Additional object contracts

These suites use the same GSAE01 assembly reference and synthetic-fixture rules above. Callback
fakes check arguments and record observable requests; they do not implement rendering, audio,
collision detection, map streaming, or actual object destruction.

| Test | Retail reference under `dlls/objects/` | Contract checked |
|------|----------------------------------------|------------------|
| `sidekick_ball` | `245_SidekickBal/SidekickBal.s`, `sidekickBall_launch` at `0x801796BC`, `sidekickBall_setIdle` at `0x80179678`, activity helpers at `0x8017962C–0x80179674` | Launch retains all three supplied velocities and copies the starting position into collision history. Modes 2/3 refresh the fade timer; modes 1/2 satisfy the held-or-moving query. Idle clears fade/path activity and disables collision. The fixture sets `fhConfigRevision()` to 0 for this GSAE01 path; it does not exercise revision-1 attachment. |
| `log_fire` | `448_DIMLogFire/DIMLogFire.s`, `DIMLogFire_free` at `0x801B07C0`, countdown at `0x801B0784` | Release the full effect-owner pointer, free a present child only for free mode 0, unregister group `0x31`, and free a present light. Strength countdown reports completion at zero or below and retains an overshot negative value. |
| `drakor_missile` | `610/610.s`, request-free at `0x80217F40`, abort at `0x80217F24`, fadeout query at `0x802183F4` | Destruction ORs flag 1 with existing flags and frees immediately only in state 1. Cancellation changes straight flight (3) to exploding (2), leaving the other states alone. |
| `rolling_barrel` | `381/381.s`, `rollingBarrel_free` at `0x801A5F80`, render at `0x801A600C` | Search group `0x2F` by complete object identity and stop after the first match. Decrement the explosion count only in state 1. Submit the model only when visible and in state 0. |
| `falling_ladder` | `266_Fall_Ladder/Fall_Ladder.s`, update `0x80187F80–0x80188178`, init `0x8018817C–0x80188240` | Read signed placement height and game bits; wait ten integer frames; play the armed sound once. Gravity is 0.9 per update, displacement uses `timeDelta`, and floor bounce scales upward velocity by 0.3. Variant `0x548` selects sequence 0 for trigger-only, sequence 1 for base-only, and neither for equal bits. |
| `moving_platform` | `475/475.s`, `dll_1DB_update`, `0x801B8BB8–0x801B8EF4` | Scan all contacts for the player, distinguish continuous boarding from reboarding, and return upward after a rider leaves the bottom. Speeds cap at ±1.5; the bottom is placement Y minus 235.5. An external trigger can start an empty descent, then reverse it when cleared. Arrival and departure update the decoded boarded game bit. |
| `dust_mote_source` | `690_DustMoteSou/DustMoteSou.s`, `dustmotesou_update`, `0x802375C4–0x802377B8` | Decode the scale and game-bit gate. A `-1` gate skips lookup. Ordinary box/arced/directional bursts require all three byte parameters; variants `0x807/0x80E` require only the first two and select masked/firework effects. File-backed and owned native fixtures must produce the same emitter arguments. |
| `firefly_wander` | `523_FireFly/FireFly.s`, `firefly_pickWanderTarget`, `0x801F4D54–0x801F4EC8` | First target uses maximum amplitude without a height RNG call; later targets sample height. Radius strictly below 21 produces no radial spread, while exactly 21 calls the radius RNG. Advance the angle in `[3000,5000]`, rotate, and add the flight origin. Fixtures choose zero final rotation; the rotation fake checks that input and applies the identity transform. |
| `invisible_hit` | `241_InvHit/InvHit.s`, `InvHit_update`, `0x80177818–0x80177C14` | Damage uses a strict distance-less-than-radius comparison and visits Tricky after the player. Mode 7 frees when the helper appears in its owner's hit list. Mode 5 publishes world coordinates only with a player and target, while preserving local position history. |
| `ice_ball` | `205_IceBall/IceBall.s`, character impact `0x8015FCCC–0x8015FF60`, surface impact `0x8015FBEC–0x8015FCC8`, update `0x8015FFC8–0x8016017C` | Notify only an owner within the active object-list range: slot 8 for `0x2CB`, slot 9 for `100/0x30A`, with an extra zero argument for `0x30A`. Emit 25 variant-specific particles. A player hit takes priority over simultaneous surface contact, hides the projectile, disables collision and starts a 120-frame cooldown. Lifetime zero remains alive; negative lifetime frees it. |

The numeric platform constants were checked in the same unit's `.sdata2` at
`0x803E4B10–0x803E4B24`; ladder gravity/damping are at `0x803E3B50/0x803E3B54` and the
firefly radius threshold is at `0x803E5EC8`. No binary fixtures are stored with these tests.

Two particularly easy behaviors to accidentally change are explicit expectations here:

- Ladder delay at `0x80188040–0x80188054` truncates `timeDelta` before subtraction. A 0.5-frame
  update leaves the delay unchanged, and the update that expires the delay does not move the ladder.
- InvHit's source-file overview describes freeing when its owner no longer references it. Retail
  `0x801779DC–0x801779FC` instead branches past the free on a non-match and frees on equality.
  The test requires the instruction behavior; the description is not its oracle.

Each additional suite was validated at `-O0` and `-O3` against both current code and one deliberate
source mutation in a temporary copy. Current code passed and all mutations failed:

| Suite | Mutation caught |
|-------|-----------------|
| `sidekick_ball` | Truncate the launch object's pointer to 32 bits |
| `log_fire` | Truncate the child pointer passed to cleanup |
| `drakor_missile` | Replace existing flags instead of OR-ing the destruction request |
| `rolling_barrel` | Walk the native group list as 32-bit entries |
| `falling_ladder` | Round fractional frame time upward when decrementing the delay |
| `moving_platform` | Walk the native contact list as 32-bit entries |
| `dust_mote_source` | Read a file-backed scale as a native float |
| `firefly_wander` | Exclude radius exactly 21 from radial spreading |
| `invisible_hit` | Free on an owner hit-list non-match instead of a match |
| `ice_ball` | Notify callback slot 9 instead of slot 8 for variant `0x2CB` |

These checks establish sensitivity to the listed divergences, not complete coverage of each
object's state machine. Production game code was not changed for this coverage batch.

## Preserved retail bugs

### Pressure switch reset leaves X unchanged

`retail_pressure_reset` covers `PressureSwitchFB_animEventCallback` in
`dlls/objects/251/251.s`. At `0x8017AD10`, retail reads placement X. At `0x8017AD14`, it stores
that value to object Z (`+0x14`). It then restores saved Y at `0x8017AD1C` and writes placement
Z to object Z at `0x8017AD24`. There is no write to object X (`+0x0C`) in the reset branch.

The fixture starts at `(91, 92, 93)` with placement `(13, 17, 27)` and saved Y `45`. The required
result is `(91, 45, 27)`. A change producing the intuitive `(13, 45, 27)` breaks retail parity.
The test checks the final coordinates, not whether the redundant intermediate store survives
native compiler optimization.

### Animation placeholders remain eligible

`retail_animation_fallback` exercises `mergeTableFiles` in `main/pi_dolphin.s`
(`0x80043CE8`). The animation/model branch at `0x800442D0–0x80044364` establishes this priority:

1. A primary entry marked `0x10000000` wins, including when both banks have marked entries.
2. Otherwise, a marked secondary entry wins, with its low 24 bits and secondary flag `0x20000000`.
3. Otherwise, copy an available primary unmarked entry, then an available secondary unmarked entry.
4. With neither available, write zero.

The test covers secondary-only placeholder selection, full entries in each bank, a full/full tie,
and rebuilding after the primary bank unloads. The animation count of 3000 is set at
`0x80043D8C`; `0x8004437C–0x80044388` terminates the merged table with `-1`.

This selection rule contributes to the confirmed [SharpClaw disguise ledge-grab T-pose](../docs/RETAIL_BUGS.md#sharpclaw-disguise-ledge-grab-t-pose).
The fixture uses invented entry offsets `0x1110/0x2220` at animation index 7. It proves the merge
keeps an unmarked placeholder when no full entry is resident, not that a particular asset has one
frame or that a map transition renders a T-pose. Those remain outside this unit test's scope.

Both quirk tests passed with the current implementation at `-O0` and `-O3`. Temporary source
mutations that reset X or discard the secondary unmarked entry failed their corresponding
assertions at both optimization levels. Those mutations were verification experiments, not
production fixes.
