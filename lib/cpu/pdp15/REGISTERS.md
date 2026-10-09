# PDP-15 Registers and State

- **PC:** program counter, interpreted using the selected model's addressing rules.
- **AC:** 18-bit accumulator state.
- **Link/condition state:** modeled only as specified for the selected PDP-15 instruction set.
- **IR:** internal instruction register for execution.
- **Memory address/data state:** internal latches used by the emulator.
- **I/O state:** active device/function, completion, and error information.

Do not expose internal implementation latches as architectural registers. Any additional registers, option-dependent state, reset values, or interrupt state must be defined in the selected machine profile and reliable instruction documentation. State snapshots must include pending I/O and all guest-visible state.