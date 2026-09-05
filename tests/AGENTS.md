# Test conventions

- Use `// Given ...`, `// When ...`, and `// Then ...` comments in test cases, as explicitly requested by the user. This is the test-specific exception to the repository's comment rule.
- State the setup, action, and observable result in those comments. Describe the scenario rather than paraphrasing each line.
- Exercise the actual game or port implementation. Use small deterministic fakes for its external dependencies.
- Use `FH_CHECK` for assertions so checks also run in optimized builds with `NDEBUG` defined.
- Keep fixtures synthetic and independent of disc images, extracted assets, and saves.
- Retail behavior is the contract, including confirmed retail bugs. Do not change an expectation to a preferred gameplay result or fix a bug that retail also exhibits.
- Establish expected behavior from retail assembly or a controlled retail runtime observation before adding a game behavior assertion. Record the game revision, function/address, evidence, and scope in `RETAIL_EVIDENCE.md`; decomp source alone is not proof.
- For a port regression, verify the test fails when the portability mismatch is restored. For a retail quirk, verify it fails when that quirk is "fixed." Test observable behavior rather than duplicating the implementation. Native undefined behavior, truncation, or layout drift is not a retail quirk to preserve; preserve the result of the retail instructions.
- Keep each test independent of other tests and reset any state needed by a second scenario.
