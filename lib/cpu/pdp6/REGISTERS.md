# PDP-6 Registers and State

The profile distinguishes verified architectural state from emulator-internal state.

- **PC / control state:** exact representation and update rules must be supplied by the selected PDP-6 profile.
- **General/accumulator registers:** represent the verified PDP-6 register set using 36-bit values.
- **IR:** internal instruction register for decode and execution.
- **Memory address/data state:** internal latches for memory operations.
- **I/O state:** device selection, request/function, completion, and error status.

Do not expose implementation latches as guest-visible registers. Preserve all architecturally relevant registers, memory, and pending I/O during save/restore. If exact reset values are unknown, report them as unspecified rather than guessing.