# Fairchild Clipper Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Register file.
4. Integer execution.
5. Branch/control unit.
6. Optional floating-point unit.
7. Load/store unit.
8. MMU/TLB where implemented.
9. Cache interface.
10. Exception/interrupt control.
11. System-bus interface.
12. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> registers;
- registers -> execution;
- execution -> load/store;
- MMU/cache -> bus;
- branch -> fetch;
- exceptions -> processor state;
- DMA -> arbitration.

## Accuracy boundary

Clipper implementation generations are represented by profiles rather than one invented internal circuit.
