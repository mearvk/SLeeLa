# Motorola 6809 Registers

- A and B: 8-bit accumulators
- D: combined 16-bit A:B register view
- X and Y: 16-bit index registers
- U: user stack pointer
- S: system stack pointer
- PC: 16-bit program counter
- DP: direct-page register
- CC: condition-code register

The CC tracks the architecture's condition and interrupt-mask state. SLeeLa preserves register aliases and stack distinctions in the model.
