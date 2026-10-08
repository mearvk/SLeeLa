# Intel iAPX 432 Circuit Model

## Functional scope

Architectural/functional model only; this is not a transistor or microcode-ROM reconstruction.

## Major blocks

1. Instruction/control interpreter.
2. Object/descriptor unit.
3. Protection and capability checks.
4. Operand/reference unit.
5. Execution unit.
6. Memory-management interface.
7. Fault/interrupt controller.
8. System interface.
9. Clock/sequencing.

## Couplers

SLCoupler models:

- control -> object resolution;
- object resolution -> operand access;
- protection -> memory transaction;
- execution -> architectural state;
- fault -> control state;
- DMA -> system arbitration.

The model intentionally preserves the unusual 432 architecture instead of translating it into a generic x86 register machine.
