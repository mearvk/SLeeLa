# DEC VAX Circuit Model

## Functional scope

This is an architectural/functional circuit model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Variable-length decoder.
3. Operand/address generator.
4. Register file.
5. Integer execution.
6. Floating-point execution.
7. String/character execution.
8. Memory-management unit.
9. Cache interface where implemented.
10. Exception/interrupt control.
11. Bus interface.
12. Clock and sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> operand generator;
- registers -> execution units;
- operand generator -> memory management;
- memory/cache -> bus;
- execution -> condition state;
- exception controller -> processor state;
- DMA -> bus arbitration.

## Accuracy boundary

VAX generations are represented as profiles rather than one invented microarchitecture.
