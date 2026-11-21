# Motorola ColdFire Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Register file.
4. Effective-address unit.
5. Integer execution.
6. Optional MAC/DSP.
7. Optional FPU.
8. Optional MMU/TLB.
9. Optional I/D cache.
10. Exception/interrupt control.
11. Debug/trace interface.
12. Embedded bus interface.
13. Clock/pipeline control.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register file;
- registers -> execution units;
- address generation -> MMU/cache;
- cache -> bus;
- MAC/FPU -> completion;
- exception -> processor state;
- DMA -> arbitration.

## Accuracy boundary

ColdFire V1-V4e are not collapsed into one invented microarchitecture.

## Executable circuit model

`SLColdFireCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLColdFireCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
