# PDP-7 Registers and State

- **PC:** program counter, with width and wrap behavior configured for the target machine.
- **AC:** 18-bit accumulator model.
- **Condition/link state:** model only behavior documented for the selected PDP-7 instruction set.
- **IR:** internal instruction register.
- **Memory address/data state:** internal latches for memory transfers.
- **I/O state:** current device/function request, completion, and error state.

Separate implementation details from programmer-visible registers. Exact reset values, interrupt behavior, and any additional registers must be backed by the selected machine profile. State snapshots must preserve all architecturally relevant state and pending I/O.