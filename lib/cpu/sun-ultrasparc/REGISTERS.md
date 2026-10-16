# Sun UltraSPARC Registers

The programmer-visible register model is a RISC general-purpose register file.

**Registers:** SPARC V9 register windows (global + windowed in/out/local), g0 hardwired zero.

The model exposes 32 addressable register slots backed by SLRegisterFile. Register slot 0 is modeled as a hardwired zero (reads 0, writes discarded), per the architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/UltraSPARC
- https://en.wikipedia.org/wiki/SPARC
