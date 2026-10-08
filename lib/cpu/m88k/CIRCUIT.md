# Motorola 88000 Circuit Model

## Scope

Functional architectural/microarchitectural model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. General register file.
4. Control-register block.
5. Integer ALU.
6. Shift/multiply/divide units.
7. Branch/control unit.
8. Floating-point execution.
9. Load/store unit.
10. Scoreboard/issue logic.
11. MMU/TLB.
12. Cache hierarchy.
13. Trap/interrupt controller.
14. System interconnect.
15. Clock/control.

## Couplers

SLCoupler models:

- decoder -> register file;
- registers -> integer/FP units;
- branch -> PC/control;
- load/store -> MMU;
- MMU -> TLB/cache;
- scoreboard -> execution units;
- control registers -> MMU/interrupt state;
- cache -> interconnect;
- DMA -> arbitration;
- exception -> processor state.

## Accuracy boundary

No MC88100/MC88110 transistor topology, cache geometry, or exact pipeline timing is claimed by generic m88k.
