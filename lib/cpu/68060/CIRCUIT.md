# Motorola 68060 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Instruction buffer.
3. Decoder.
4. Branch prediction/control.
5. Dispatch/issue.
6. Data/address register files.
7. Integer execution units.
8. Address-generation units.
9. Floating-point unit.
10. Instruction cache.
11. Data cache.
12. MMU/translation.
13. Exception/interrupt control.
14. Burst bus interface.
15. Completion/control.

## Couplers

SLCoupler models:

- fetch -> instruction buffer;
- decoder -> dispatch;
- branch predictor -> fetch/dispatch;
- register files -> execution units;
- address generation -> MMU;
- MMU -> caches;
- FPU -> completion;
- D-cache -> bus;
- exception -> processor state;
- DMA -> bus arbitration.

## Accuracy boundary

No undocumented transistor-level topology or exact cycle schedule is claimed.
