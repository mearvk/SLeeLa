# DEC VAX Circuit Model

## Functional scope

This is an architectural/functional circuit model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Variable-length decoder.
3. Operand/address generator.
4. Register file.
5. Integer execution.
6. Floating-point execution.
7. String/character execution.
8. Memory-management unit.
9. Cache interface where implemented.
10. Exception/interrupt control.
11. Bus interface.
12. Clock and sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> operand generator;
- registers -> execution units;
- operand generator -> memory management;
- memory/cache -> bus;
- execution -> condition state;
- exception controller -> processor state;
- DMA -> bus arbitration.

## Accuracy boundary

VAX generations are represented as profiles rather than one invented microarchitecture.

## Executable circuit model

`SLVAXCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLVAXCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
