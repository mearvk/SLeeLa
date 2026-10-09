# LINC-8 Registers and State

Common emulation state:
- **PC:** program counter, with behavior defined by the active mode and machine profile.
- **AC:** 12-bit accumulator state where applicable to the active execution personality.
- **Mode:** explicit LINC/PDP-8 execution selector.
- **IR:** internal instruction register.
- **Memory address/data state:** internal state for memory transfers.
- **LINC-specific state:** represented separately and only where defined by the selected LINC profile.
- **I/O state:** device selection, pending operation, completion, or error.

Do not assume that similarly named registers have identical semantics across both modes. Reset values and mode-switch side effects must be documented by the machine profile.