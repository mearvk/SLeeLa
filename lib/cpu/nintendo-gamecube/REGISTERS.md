# Nintendo GameCube (Gekko / PowerPC) Registers

The programmer-visible register model is a RISC general-purpose register file.

**Registers:** PowerPC 750-class register model (32 GP, 32 FP) with on-chip L1/L2; IBM Gekko.

The model exposes 32 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/Gekko_(microprocessor)
- https://en.wikipedia.org/wiki/PowerPC_7xx
