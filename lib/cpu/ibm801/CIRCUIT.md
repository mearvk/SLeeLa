# IBM 801 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Fixed-format decoder.
3. Register file.
4. Integer execution.
5. Branch/control unit.
6. Load/store interface.
7. Processor-memory interface.
8. Exception/control logic.
9. System-bus interface.
10. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register file;
- registers -> execution;
- execution -> memory;
- branch -> fetch;
- exception -> processor state;
- DMA -> arbitration.

## Accuracy boundary

The IBM 801 is deliberately modeled as its own early RISC design without importing undocumented later IBM CPU internals.
