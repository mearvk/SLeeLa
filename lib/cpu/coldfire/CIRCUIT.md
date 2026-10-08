# Motorola ColdFire Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Register file.
4. Effective-address unit.
5. Integer execution.
6. Optional MAC/DSP.
7. Optional FPU.
8. Optional MMU/TLB.
9. Optional I/D cache.
10. Exception/interrupt control.
11. Debug/trace interface.
12. Embedded bus interface.
13. Clock/pipeline control.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register file;
- registers -> execution units;
- address generation -> MMU/cache;
- cache -> bus;
- MAC/FPU -> completion;
- exception -> processor state;
- DMA -> arbitration.

## Accuracy boundary

ColdFire V1-V4e are not collapsed into one invented microarchitecture.
