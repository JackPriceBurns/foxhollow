# Game regression tests

These tests run real game functions with synthetic objects and small C fakes for dependencies
such as audio, object lookup, and message queues. They do not need a disc image, a save, a window,
or the renderer. The standalone build uses the checked-out Aurora headers without configuring
Aurora or fetching Dawn.

## Retail is the contract

Tests must preserve what the original game does, including its bugs. Expected behavior comes from
retail assembly or controlled observations in retail, with provenance recorded in
[RETAIL_EVIDENCE.md](RETAIL_EVIDENCE.md). Matching the decomp alone is insufficient.

For a port regression, the test should fail when a portability mismatch is reintroduced. For a
confirmed retail quirk, the test should fail if someone "fixes" the quirk. The latter are labeled
`quirk` and can be run with `ctest --test-dir build-tests -L quirk --output-on-failure`.
An unexpected result is a reason to consult retail before changing code or the expected value.
Preserve retail's observable result through defined native C; host pointer truncation, undefined
behavior, and layout drift are portability problems, not retail quirks.

For example, `retail_pressure_reset` deliberately requires a displaced pressure switch to keep
its X coordinate after reset. Retail writes to Z twice and never resets X. Changing that to the
more intuitive X/Y/Z reset must fail the test.

## Running

Use a 64-bit compiler and an initialized `extern/aurora` submodule:

```sh
cmake -S tests -B build-tests -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

Use a second build directory with `-DCMAKE_BUILD_TYPE=Debug` to test an unoptimized build.
The GitHub Actions workflow runs Debug and Release on macOS and Linux. Local validation was
performed on macOS arm64; CI provides the Linux check after the workflow is pushed.

To run just one regression:

```sh
ctest --test-dir build-tests -R thornbush_hits --output-on-failure
```

Use `-L object` to select the object suites or `-L endian` to select object placement decoding
coverage. Each executable can contain multiple scenarios, including boundary cases and repeated
updates; the CTest count is the number of executables, not the number of scenarios.

The tests can also be included in a normal game build:

```sh
cmake -B build -G Ninja -DFOXHOLLOW_BUILD_TESTS=ON
cmake --build build --target foxhollow_tests
ctest --test-dir build -R '^foxhollow\.' --output-on-failure
```

The standalone build is the faster entry point when working only on tests.

## Given / When / Then

Each case describes its setup, action, and expected result with comments:

```c
// Given a thornbush with ten health and a colliding attacker allocated above 4 GB.
state->health = 10;

// When the same attacker remains in contact across two collision updates.
drakord_thornbush_hitDetect(&obj);
drakord_thornbush_hitDetect(&obj);

// Then only the first contact damages the thornbush.
FH_CHECK(state->health == 9);
```

The complete fixture is in `thornbush_hits_test.c`. A later When/Then pair ends contact and
hits again, checking that the thornbush can take damage from a new contact. The test establishes
observable gameplay behavior; it does not assert how the function implements it.

`FH_CHECK` remains active when `NDEBUG` is defined and reports the failed expression, file,
and line. `FH_UNEXPECTED_CALL` fails immediately if a dependency that should not be reached is
called. `fh_test_alloc_high` verifies allocations are above 4 GB so pointer truncation cannot
silently pass on an allocator returning low addresses.

`FH_CHECK_NEAR` accepts an explicit absolute tolerance for floating-point arithmetic. Use exact
checks for coordinates that are simply copied and for exactly representable decoded constants.
`fh_test_write_be16` and `fh_test_write_be32` build synthetic file-backed fields byte by byte;
they do not call the production endian readers or writers. Keep the placement base header native
and encode only the object-specific payload, matching the loader's contract.

## Structure

Each executable includes one actual game `.c` file and provides only the external functions
needed by the scenario. This allows tests to exercise existing code and inspect private records
without changing production interfaces just for testing. The linker removes unrelated functions
from that translation unit. These are focused regression tests, not a replacement implementation
of the game.

Each test runs in a separate process, isolating game globals and allowing CTest to report a crash
as one failed test. The build uses the native game headers, signed-char convention, `TARGET_PC=1`,
`VERSION_GSAE01`, and the port compatibility header. It does not link the game executable.

| Test | Regression covered |
|------|--------------------|
| `pressure_switch` | Tracking and resetting all ten native object pointers |
| `grimble_path` | Selecting a path from a native pointer array and calling its interface |
| `sequence_messages` | Relaying three message kinds to the correct object |
| `effect_cleanup` | Passing the complete owner address to effect cleanup |
| `deferred_messages` | Keeping adjacent message records and sender pointers intact |
| `enemy_path_cleanup` | Freeing and clearing the original path allocation |
| `thornbush_hits` | Counting damage once per attacker contact |
| `retail_pressure_reset` | Preserving retail's failure to reset the pressure switch's X coordinate |
| `retail_animation_fallback` | Preserving placeholder selection and full-bank priority behind the disguise T-pose |
| `sidekick_ball` | Launch velocities, collision setup, keep-alive modes and returning Tricky's ball to idle |
| `log_fire` | Child and light cleanup by free mode, plus strength exhaustion and overshoot |
| `drakor_missile` | Deferred destruction flags, immediate fadeout cleanup and straight-flight cancellation |
| `rolling_barrel` | Native group membership, explosion accounting and render visibility by lifecycle state |
| `falling_ladder` | Placement decoding, integer fall delay, gravity, bounce and two-bit sequence selection |
| `moving_platform` | Rider contacts, reboarding, upper/lower stops, speed caps and trigger-controlled reversal |
| `dust_mote_source` | File-backed versus owned placement scale, game-bit gates and all five effect paths |
| `firefly_wander` | First-target height, seeded later targets and the radius-21 spread boundary |
| `invisible_hit` | Exclusive damage radius, owner-hit cleanup and lock-on position publishing |
| `ice_ball` | Active-owner callback slots, particle variants, character-impact priority and lifetime expiry |

The animation fallback fixture covers table selection with synthetic entries. It does not load
the disguise model, replay map streaming, or verify the rendered T-pose. That distinction and the
known retail behavior are documented in the [retail bug archive](../docs/RETAIL_BUGS.md).

## Adding coverage

Add a `*_test.c` file with Given/When/Then comments and register it with
`foxhollow_add_regression` in `CMakeLists.txt`. Prefer a small scenario reproducing a known failure.
Record the retail evidence and verify that the relevant behavioral divergence makes the test fail:
restore the port bug, or "fix" the preserved retail bug, in a temporary source copy. Run both Debug
and Release.
Keep random choices and frame timing deterministic. Add independent cases as separate executables,
or reset fixture state explicitly between scenarios.

Good next areas are endian readers, packed numeric conversions, shared structure layouts, and
object lifetime transitions. Their fixtures can be generated directly in the test without retail
data. AddressSanitizer and UndefinedBehaviorSanitizer builds would add memory and arithmetic checks;
they are not enabled by this starter suite.

Full-game replay tests are a separate layer: boot with a locally supplied disc and disposable save,
control input, random state, and timing, then inspect checkpoints such as player position, game bits,
or spawned objects. Rendering comparisons also need controlled graphics settings and tolerances.
Those tests take more infrastructure and are not implemented here. Do not commit retail-derived
fixtures or personal saves.
