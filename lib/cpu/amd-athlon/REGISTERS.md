# AMD Athlon (K7) Registers

The programmer-visible register model is a CISC named/split register set.

**Registers:** IA-32 register model (K7 microarchitecture) with on-chip L1 and L2.

The model exposes 8 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/Athlon
- https://en.wikipedia.org/wiki/List_of_AMD_processors
