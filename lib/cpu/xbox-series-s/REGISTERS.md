# Xbox Series S (x86-64 Zen 2) Registers

The programmer-visible register model is a CISC named/split register set.

**Registers:** x86-64 register model; 8-core AMD Zen 2 cluster with shared L3.

The model exposes 16 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/Xbox_Series_X_and_Series_S
- https://en.wikipedia.org/wiki/Zen_2
