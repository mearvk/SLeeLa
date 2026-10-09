# PDP-1 Registers and State

Model architectural state separately from emulator implementation state.

- **PC:** program counter, with width and wrap behavior specified by the selected memory profile.
- **AC:** 18-bit accumulator model.
- **IO / device state:** represent I/O data and status only as required by the verified PDP-1 instruction and device definitions.
- **IR:** internal instruction register used by the emulator.
- **Memory address/data latches:** internal execution state.

Do not invent programmer-visible registers or flags. Reset behavior, arithmetic edge cases, and state transitions must be driven by the verified PDP-1 specification. Save/restore preserves all architectural state and pending I/O.