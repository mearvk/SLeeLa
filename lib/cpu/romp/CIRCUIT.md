# IBM ROMP Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Register file.
4. Integer execution.
5. Branch/control unit.
6. Load/store unit.
7. Memory-management interface where implemented.
8. Instruction/data cache interface.
9. Exception/interrupt controller.
10. System-bus interface.
11. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> registers;
- registers -> execution;
- execution -> memory;
- memory management/cache -> bus;
- branch -> fetch;
- exception -> processor state;
- DMA -> arbitration.

## Accuracy boundary

ROMP remains a separate architecture and is not given undocumented POWER-family internals.
