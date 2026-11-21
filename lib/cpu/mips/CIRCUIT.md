# MIPS Circuit Model

## Scope

This is a functional CPU circuit model, not a transistor-level reconstruction.

## Major blocks

1. 32-entry general-register file.
2. Constant-zero enforcement for R0.
3. Program counter.
4. Instruction fetch/decode.
5. Immediate/sign/zero extension.
6. ALU and comparator.
7. Shifter.
8. Multiply/divide path with HI/LO.
9. Pipeline stage registers.
10. Load/store address and data paths.
11. Branch/jump control.
12. CP0/system-control path.
13. Exception/interrupt control.
14. External bus interface.
15. Clock/control logic.

## Couplers

SLCoupler models:

- register file -> ALU;
- ALU -> register file;
- register file -> address generation;
- memory -> writeback;
- branch comparator -> PC control;
- CP0 -> exception control;
- pipeline stage -> pipeline stage.

## Accuracy boundary

No particular cache, TLB, FPU, bus protocol, pipeline depth beyond the functional abstraction, or transistor layout is claimed unless supported by a concrete MIPS implementation source.

## Executable circuit model

`SLMIPSCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLMIPSCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **B** (7/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
