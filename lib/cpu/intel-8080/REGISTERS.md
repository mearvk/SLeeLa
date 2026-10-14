# Intel 8080 Registers

The programmer-visible register model is a accumulator / indexed register machine.

**Registers:** A, B, C, D, E, H, L accumulator/register file, SP, PC, flags.

The model exposes 8 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/Intel_8080
- https://en.wikipedia.org/wiki/X86
