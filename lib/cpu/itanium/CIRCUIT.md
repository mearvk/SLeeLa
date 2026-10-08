# Intel Itanium Circuit Model

## Scope

Functional architectural/microarchitectural model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. 128-bit bundle buffer.
3. Template/slot decoder.
4. General register file.
5. Floating-point register file.
6. Predicate register file.
7. Branch register file.
8. Rotating-register/RSE machinery.
9. Integer execution clusters.
10. Floating-point execution clusters.
11. Branch units.
12. Load/store units.
13. Speculation/check machinery.
14. MMU/TLB.
15. Cache hierarchy.
16. Interrupt/exception controller.
17. IA-32 compatibility subsystem where enabled.
18. System interconnect.
19. Clock/control.

## Couplers

SLCoupler models:

- bundle -> decoder;
- template -> slot classification;
- predicates -> execution units;
- registers -> execution units;
- execution -> register files;
- branch -> control state;
- RSE -> register stack;
- load/store -> MMU;
- MMU -> TLB/cache;
- speculation -> check/recovery;
- cache -> interconnect;
- DMA -> arbitration.

## Accuracy boundary

No specific Itanium generation's transistor layout, exact cluster topology, cache geometry, or pipeline depth is asserted by the generic architecture.
