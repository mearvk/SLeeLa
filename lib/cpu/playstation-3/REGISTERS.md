# PlayStation 3 (Cell / PowerPC) Registers

The programmer-visible register model is a RISC general-purpose register file.

**Registers:** Power ISA PPE register model (32 GP, 32 FP, AltiVec) plus SPE array; Cell BE.

The model exposes 32 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/Cell_(processor)
- https://en.wikipedia.org/wiki/Comparison_of_PlayStation_consoles
