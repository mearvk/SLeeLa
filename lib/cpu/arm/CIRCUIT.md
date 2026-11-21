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

## Executable circuit model

`SLARMCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLARMCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
