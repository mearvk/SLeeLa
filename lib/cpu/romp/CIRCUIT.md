# IBM ROMP Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Register file.
4. Integer execution.
5. Branch/control unit.
6. Load/store unit.
7. Memory-management interface where implemented.
8. Instruction/data cache interface.
9. Exception/interrupt controller.
10. System-bus interface.
11. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> registers;
- registers -> execution;
- execution -> memory;
- memory management/cache -> bus;
- branch -> fetch;
- exception -> processor state;
- DMA -> arbitration.

## Accuracy boundary

ROMP remains a separate architecture and is not given undocumented POWER-family internals.

## Executable circuit model

`SLROMPCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLROMPCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
