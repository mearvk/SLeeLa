# Intel 4004 Registers and State

- **PC:** 12-bit program counter.
- **Accumulator (A):** 4-bit arithmetic accumulator.
- **Carry (CY):** one-bit arithmetic/test state.
- **R0–R15:** sixteen 4-bit registers; register-pair operations treat adjacent registers as eight pairs.
- **Return stack:** three 12-bit return addresses, with hardware-defined nesting/rotation semantics.
- **Instruction register:** internal 8-bit instruction-byte latch.
- **Chip-select and I/O state:** internal emulator state for external chipset operations.

Keep programmer-visible state separate from internal implementation latches. Reset, stack rotation, and flag behavior should follow the selected 4004 specification. State snapshots must preserve all registers, PC, carry, stack, and pending I/O.