# PlayStation 2 (Emotion Engine / MIPS) Registers

The programmer-visible register model is a RISC general-purpose register file.

**Registers:** MIPS III/IV register model (32 128-bit GP, r0 zero) in the Emotion Engine.

The model exposes 34 addressable register slots backed by SLRegisterFile. Register slot 0 is modeled as a hardwired zero (reads 0, writes discarded), per the architecture.

## Status / condition state

Condition/zero/negative state is tracked by the model's flag fields (flagZ/flagN) and any architecture status register listed above. Flags are updated by ALU and compare operations.

## Internal state

The physical device contains internal latches and buses used during address and data movement. SLeeLa promotes an internal element to a modeled register only when it affects bus visibility, cycle timing, interrupt restart, address formation, or instruction sequencing.

## Sources

- https://en.wikipedia.org/wiki/Emotion_Engine
- https://en.wikipedia.org/wiki/Comparison_of_PlayStation_consoles
