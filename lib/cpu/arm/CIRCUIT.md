# ARM Circuit Model

## Scope

Functional architectural/microarchitectural boundary model, not a transistor-level reconstruction.

## Major blocks

1. General-register file.
2. Program counter.
3. PSTATE/CPSR control state.
4. Instruction fetch.
5. A32/T32/A64 decoder.
6. Integer ALU.
7. Address-generation/load-store units.
8. Branch/control unit.
9. FP/SIMD/NEON unit.
10. Pipeline/issue/completion state.
11. MMU/TLB and translation walk.
12. Cache hierarchy.
13. System-register interface.
14. Exception/interrupt controller.
15. Security/privilege state.
16. System interconnect.
17. Clock/control.

## Couplers

SLCoupler models:

- decoder -> execution unit;
- register -> ALU;
- register -> address generation;
- load/store -> MMU;
- MMU -> TLB/cache;
- branch -> PC;
- system register -> MMU/exception state;
- exception -> vector state;
- cache -> interconnect;
- DMA -> bus arbitration.

## Accuracy boundary

No Cortex-specific pipeline width, cache geometry, branch predictor, execution-port count, or transistor topology is claimed by the generic ARM model.
