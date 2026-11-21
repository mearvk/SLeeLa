# DEC Alpha Circuit Model

## Scope

Functional architectural/microarchitectural model, not transistor-level reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Integer register file.
4. Floating-point register file.
5. Integer execution units.
6. Shift/byte-manipulation unit.
7. Integer multiply/divide.
8. Floating-point unit.
9. Branch/control unit.
10. Load/store unit.
11. Issue/scheduling structures.
12. MMU/TLB.
13. Cache hierarchy.
14. PAL/system-control interface.
15. Exception/interrupt controller.
16. Retirement.
17. System interconnect.
18. Clock/control.

## Couplers

SLCoupler models:

- decoder -> register files;
- registers -> execution units;
- execution -> register files;
- branch -> PC;
- load/store -> MMU;
- MMU -> TLB/cache;
- issue -> execution units;
- execution -> retirement;
- PAL -> privileged state;
- cache -> interconnect;
- DMA -> bus arbitration;
- exception -> system state.

## Accuracy boundary

No EV-specific transistor layout, cache geometry, issue width, branch predictor, or pipeline depth is assigned to generic Alpha.

## Executable circuit model

`SLAlphaCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLAlphaCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
