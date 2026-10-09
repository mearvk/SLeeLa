# TX-0 Registers and State

The profile separates machine-visible state from emulator-internal state.

- **PC:** program counter, width and wrap rules configured for the selected TX-0 profile.
- **Accumulator / arithmetic state:** modeled only according to the selected instruction reference.
- **Instruction register:** internal state for the current instruction.
- **Memory address/data state:** internal transfer state.
- **Console and device state:** pending input/output, completion, and error conditions.

The TX-0's exact programmer-visible register semantics must be filled from the chosen primary reference. Do not invent flag names, reset values, or extra registers. Save/restore must preserve all state needed for deterministic continuation.