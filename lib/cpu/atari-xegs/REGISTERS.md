# Atari XEGS (6502C) Registers

The programmer-visible register model is a accumulator / indexed register machine.

**Registers:** 6502-class model (A, X, Y, S, PC, P); MOS 6502C (SALLY).

The model exposes 5 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/Atari_XEGS
- https://en.wikipedia.org/wiki/MOS_Technology_6502
