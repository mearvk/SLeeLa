# IBM 801 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Fixed-format decoder.
3. Register file.
4. Integer execution.
5. Branch/control unit.
6. Load/store interface.
7. Processor-memory interface.
8. Exception/control logic.
9. System-bus interface.
10. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register file;
- registers -> execution;
- execution -> memory;
- branch -> fetch;
- exception -> processor state;
- DMA -> arbitration.

## Accuracy boundary

The IBM 801 is deliberately modeled as its own early RISC design without importing undocumented later IBM CPU internals.

## Executable circuit model

`SLIBM801Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLIBM801CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
