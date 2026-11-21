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

## Executable circuit model

`SLPowerPCCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPowerPCCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
