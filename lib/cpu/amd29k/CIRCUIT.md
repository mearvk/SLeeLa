# AMD 29000 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Logical register/window mapper.
4. Register file.
5. Integer execution.
6. Optional floating-point unit.
7. Load/store unit.
8. MMU/TLB where implemented.
9. Cache interface.
10. Branch/control unit.
11. Exception/interrupt controller.
12. System-bus interface.
13. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register mapper;
- register mapper -> execution;
- execution -> load/store;
- MMU/cache -> bus;
- branch -> fetch;
- exceptions -> processor state;
- DMA -> arbitration.

## Accuracy boundary

29000 generations remain separate profiles rather than one invented universal microarchitecture.
