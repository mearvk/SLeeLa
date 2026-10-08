# NS32000 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Variable-length decoder.
3. Operand/effective-address unit.
4. Register file.
5. Integer ALU.
6. Multiply/divide unit.
7. Optional floating-point interface.
8. Memory-management interface.
9. Cache interface where implemented.
10. Interrupt/exception controller.
11. Bus interface.
12. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> operand unit;
- registers -> ALU;
- address generation -> MMU/cache;
- cache -> bus;
- execution -> status;
- exceptions -> processor state;
- DMA -> bus arbitration.

## Accuracy boundary

32000 generations remain separate profiles rather than one invented universal microarchitecture.
