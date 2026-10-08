# PowerPC Circuit Model

## Scope

Functional architectural circuit model; not a transistor-level reconstruction.

## Major blocks

1. 32-entry GPR file.
2. 32-entry FPR file.
3. Branch processor.
4. CR field logic.
5. LR/CTR branch state.
6. XER arithmetic status.
7. Integer/fixed-point execution unit.
8. Floating-point execution unit when implemented.
9. Load/store unit.
10. Instruction fetch/decode/dispatch.
11. Pipeline and completion state.
12. MMU/TLB when implemented.
13. Instruction/data cache interfaces when implemented.
14. Exception/interrupt control.
15. System bus interface.
16. Clock and sequencing.

## Couplers

SLCoupler models:

- GPR -> integer unit;
- FPR -> floating-point unit;
- ALU -> GPR;
- compare -> CR;
- branch unit -> PC;
- LR/CTR -> branch control;
- load/store -> cache/bus;
- MSR/SPR -> privileged control;
- exception -> vector/control state.

## Accuracy boundary

No implementation-specific cache, TLB, bus width, pipeline depth, or execution-unit count is asserted without a concrete processor source.
