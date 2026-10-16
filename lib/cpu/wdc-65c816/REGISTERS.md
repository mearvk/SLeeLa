# WDC 65C816 Registers

The programmer-visible register model is a accumulator / indexed register machine.

**Registers:** A (8/16), X/Y (8/16), 16-bit stack pointer, direct-page, 24-bit program address, status.

The model exposes 6 addressable register slots backed by SLRegisterFile. No register slot is hardwired to zero in this architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://wikiwand.com/en/articles/65C816
