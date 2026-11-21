# Inmos Transputer Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Operand/workspace evaluation.
4. Integer execution.
5. Optional floating-point execution.
6. Process scheduler.
7. Channel controller.
8. Serial link interfaces.
9. Local-memory interface.
10. Timer/event logic.
11. Interrupt/control logic.
12. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> operand evaluation;
- execution -> process state;
- scheduler -> process execution;
- channels -> links;
- links -> external interconnect;
- memory -> execution;
- DMA -> system arbitration where present.

## Accuracy boundary

Transputer concurrency and communication behavior are modeled explicitly without inventing undocumented internal transistor topology.

## Executable circuit model

`SLTransputerCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLTransputerCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
