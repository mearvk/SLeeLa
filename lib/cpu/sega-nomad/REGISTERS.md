# Sega Nomad (68000) Registers

The programmer-visible register model is a CISC named/split register set.

**Registers:** 68000 register model (D0-D7, A0-A7, PC, SR); Z80 sound sub-CPU.

The model exposes 16 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/Sega_Nomad
- https://en.wikipedia.org/wiki/Sega_Genesis
