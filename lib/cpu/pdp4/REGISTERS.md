# PDP-4 Registers and State

The emulator state is deliberately split between architecture-visible state and internal implementation state.

- **PC:** program counter, sized and wrapped according to the selected PDP-4 machine profile.
- **AC:** 18-bit accumulator model.
- **Link/condition state:** represent only the flags or link behavior established by the selected instruction specification.
- **IR:** internal instruction register used during execution.
- **Memory address/data latches:** internal state for memory operations.
- **I/O request state:** selected device/function and completion or error status.

Do not expose internal latches as programmer-visible registers. Reset behavior and any additional registers must be backed by the machine profile. Save/restore must preserve all architecturally relevant state and pending I/O.