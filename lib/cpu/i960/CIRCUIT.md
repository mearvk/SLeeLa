# Intel i960 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Register-cache manager.
4. Register file.
5. Integer execution.
6. Optional floating-point unit.
7. Load/store unit.
8. MMU/protection unit where implemented.
9. Instruction/data cache.
10. Branch/control unit.
11. Exception/interrupt controller.
12. Bus interface.
13. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register manager;
- registers -> execution;
- execution -> load/store;
- MMU/cache -> bus;
- protection -> exception controller;
- branch -> fetch;
- DMA -> arbitration.

## Accuracy boundary

i960 implementation generations remain separate profiles.
