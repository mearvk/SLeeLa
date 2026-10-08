# SPARC Circuit Model

## Scope

Functional architectural/microarchitectural model, not transistor-level reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Register-window file.
4. Integer unit/ALU.
5. PC/nPC control.
6. Branch/control unit.
7. Load/store address generation.
8. Floating-point unit.
9. Trap/privilege controller.
10. Window spill/fill controller.
11. MMU/TLB.
12. Cache hierarchy.
13. System interconnect.
14. Clock/control.

## Couplers

SLCoupler models:

- decoder -> window register file;
- window registers -> integer unit;
- integer unit -> window registers;
- branch -> PC/nPC;
- load/store -> MMU;
- MMU -> TLB/cache;
- SAVE/RESTORE -> window controller;
- window controller -> trap controller;
- FPU -> register state;
- trap -> privileged state;
- cache -> interconnect;
- DMA -> bus arbitration.

## Accuracy boundary

No specific SPARC implementation's transistor topology, pipeline width, cache geometry, branch predictor, or execution-port count is asserted.
