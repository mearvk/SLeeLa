# PDP-5 Registers and State

- **PC:** program counter; width and wrap behavior follow the configured 12-bit address model.
- **AC:** 12-bit accumulator.
- **Link:** one-bit arithmetic/link state, modeled separately from AC.
- **IR:** internal instruction register used by the emulator; implementation state, not necessarily programmer-visible.
- **Memory address / data latches:** internal state for memory access.
- **I/O state:** current device/function request and completion status.

Reset values and exact sequencing should be supplied by a machine profile where known. Avoid inventing undocumented visible registers. Save/restore state must preserve PC, AC, link, memory, and pending I/O.