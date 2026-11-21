# Intel i960 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Register-cache manager.
4. Register file.
5. Integer execution.
6. Optional floating-point unit.
7. Load/store unit.
8. MMU/protection unit where implemented.
9. Instruction/data cache.
10. Branch/control unit.
11. Exception/interrupt controller.
12. Bus interface.
13. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register manager;
- registers -> execution;
- execution -> load/store;
- MMU/cache -> bus;
- protection -> exception controller;
- branch -> fetch;
- DMA -> arbitration.

## Accuracy boundary

i960 implementation generations remain separate profiles.

## Executable circuit model

`SLI960Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLI960CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
