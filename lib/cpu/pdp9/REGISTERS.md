# PDP-9 Registers and State

- **PC:** program counter, with width and wrap behavior supplied by the machine profile.
- **AC:** 18-bit accumulator state.
- **Condition/link state:** model only state supported by the chosen PDP-9 instruction reference.
- **IR:** internal instruction register for the SLeeLa execution engine.
- **Memory address/data latches:** internal implementation state.
- **I/O request state:** selected device/function plus completion and error status.

Distinguish programmer-visible registers from internal latches. Reset behavior and any additional registers require a reliable machine-specific definition. Snapshots must preserve architecturally visible state, memory, and pending I/O.