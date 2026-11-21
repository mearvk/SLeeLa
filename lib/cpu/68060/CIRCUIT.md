# Motorola 68060 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Instruction buffer.
3. Decoder.
4. Branch prediction/control.
5. Dispatch/issue.
6. Data/address register files.
7. Integer execution units.
8. Address-generation units.
9. Floating-point unit.
10. Instruction cache.
11. Data cache.
12. MMU/translation.
13. Exception/interrupt control.
14. Burst bus interface.
15. Completion/control.

## Couplers

SLCoupler models:

- fetch -> instruction buffer;
- decoder -> dispatch;
- branch predictor -> fetch/dispatch;
- register files -> execution units;
- address generation -> MMU;
- MMU -> caches;
- FPU -> completion;
- D-cache -> bus;
- exception -> processor state;
- DMA -> bus arbitration.

## Accuracy boundary

No undocumented transistor-level topology or exact cycle schedule is claimed.

## Executable circuit model

`SL68060Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SL68060CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
