# PA-RISC Circuit Model

## Scope

Functional architecture/microarchitecture model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. General register file.
4. Shadow/fast-interrupt state.
5. Integer ALU.
6. Shift/bit-manipulation unit.
7. Branch/control unit.
8. Load/store unit.
9. Floating-point unit.
10. MAX multimedia unit where enabled.
11. Issue/dependency/reorder structures.
12. MMU/TLB.
13. Cache hierarchy.
14. Trap/interrupt controller.
15. System interconnect.
16. Clock/control.

## Couplers

SLCoupler models:

- decoder -> register file;
- registers -> ALU;
- ALU -> register file;
- branch -> PC;
- load/store -> MMU;
- MMU -> TLB/cache;
- FPU/MAX -> register state;
- dependency unit -> execution units;
- reorder -> retirement;
- trap -> control state;
- cache -> interconnect;
- DMA -> bus arbitration.

## Accuracy boundary

No specific PA-7000/PA-8000/PA-8900 pipeline width, cache geometry, transistor topology, or execution-port arrangement is assigned to generic PA-RISC.
