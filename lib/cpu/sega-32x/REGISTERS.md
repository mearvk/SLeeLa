# Sega 32X (SH-2) Registers

The programmer-visible register model is a RISC general-purpose register file.

**Registers:** Hitachi SH-2 register model (R0-R15, PC, SR, GBR, VBR); two SH-2 cores.

The model exposes 16 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/32X
- https://treasure.fandom.com/wiki/Sega_32X/Technical_Specifications
