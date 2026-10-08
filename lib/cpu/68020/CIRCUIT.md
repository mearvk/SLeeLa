# Motorola 68020 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction prefetch.
2. Instruction cache.
3. Decoder.
4. Effective-address generator.
5. Data register file.
6. Address register file.
7. Integer/shift unit.
8. Condition-code logic.
9. Branch/exception controller.
10. Bus interface.
11. Coprocessor interface.
12. Clock/pipeline control.

## Couplers

SLCoupler models:

- prefetch -> cache/decode;
- decode -> register files;
- address registers -> effective-address unit;
- effective address -> bus;
- ALU -> condition codes;
- exception -> supervisor/stack state;
- coprocessor -> execution/control;
- DMA -> bus arbitration.

## Accuracy boundary

No 68030 MMU, 68040 superscalar pipeline, or later 68060 implementation detail is projected backward onto the 68020.
