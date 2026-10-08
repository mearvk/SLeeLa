# Motorola 68030 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction prefetch/cache.
2. Decoder.
3. Effective-address generator.
4. Data/address register files.
5. Integer/shift unit.
6. Branch/exception controller.
7. Instruction cache.
8. Data cache.
9. MMU.
10. ATC.
11. Burst bus interface.
12. Coprocessor interface.
13. Interrupt controller.
14. Clock/pipeline control.

## Couplers

SLCoupler models:

- prefetch -> I-cache/decode;
- decode -> register files;
- address generation -> MMU;
- MMU/ATC -> caches;
- ALU -> condition codes;
- D-cache -> bus;
- exception -> supervisor state;
- coprocessor -> execution/control;
- DMA -> bus arbitration.

## Accuracy boundary

No 68040 dual-MMU, 4-KB-cache, multiple-concurrent-execution topology is projected backward onto the 68030. citeturn0search5
