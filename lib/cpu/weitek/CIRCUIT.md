# Weitek POWER Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Integer register file.
4. Floating-point register file.
5. Integer execution.
6. Floating-point execution.
7. Load/store unit.
8. Branch/control unit.
9. Cache interface.
10. Optional MMU/TLB.
11. Exception/interrupt controller.
12. System-bus interface.
13. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register files;
- register files -> execution;
- execution -> load/store;
- cache/MMU -> bus;
- branch -> fetch;
- exception -> processor state;
- DMA -> arbitration.

## Accuracy boundary

Weitek POWER remains a separate implementation profile and is not assigned IBM POWER or PowerPC internals.
