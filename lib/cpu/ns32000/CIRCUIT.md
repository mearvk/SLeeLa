# NS32000 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Variable-length decoder.
3. Operand/effective-address unit.
4. Register file.
5. Integer ALU.
6. Multiply/divide unit.
7. Optional floating-point interface.
8. Memory-management interface.
9. Cache interface where implemented.
10. Interrupt/exception controller.
11. Bus interface.
12. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> operand unit;
- registers -> ALU;
- address generation -> MMU/cache;
- cache -> bus;
- execution -> status;
- exceptions -> processor state;
- DMA -> bus arbitration.

## Accuracy boundary

32000 generations remain separate profiles rather than one invented universal microarchitecture.

## Executable circuit model

`SLNS32000Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLNS32000CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
