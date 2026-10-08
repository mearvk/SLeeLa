# RISC-V Circuit Model

## Scope

Functional architectural and microarchitectural model, not a transistor reconstruction.

## Major blocks

1. Register file.
2. Program counter.
3. Instruction fetch.
4. Decoder.
5. Immediate-generation logic.
6. Integer ALU.
7. Branch/control unit.
8. Load/store address generation.
9. Multiply/divide unit where M is enabled.
10. Atomic unit where A is enabled.
11. FP unit where F/D is enabled.
12. Vector unit where V is enabled.
13. CSR/system-control block.
14. MMU/TLB and page-table walk.
15. Cache hierarchy.
16. Interrupt/exception control.
17. Interconnect interface.
18. Clock/control.

## Couplers

SLCoupler models:

- decoder -> register file;
- register file -> ALU;
- immediate -> ALU;
- ALU -> register file;
- branch -> PC;
- load/store -> MMU;
- MMU -> TLB/cache;
- CSR -> privilege/translation state;
- extension unit -> register file;
- cache -> interconnect;
- DMA -> bus arbitration;
- exception -> trap state.

## Accuracy boundary

No particular RISC-V vendor's pipeline depth, cache geometry, branch predictor, execution-port count, or physical circuit layout is asserted by this generic implementation.
