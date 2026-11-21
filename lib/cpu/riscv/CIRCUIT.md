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

## Executable circuit model

`SLRISCVCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLRISCVCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
