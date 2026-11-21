# DEC PDP-11 Circuit Model

## Functional scope

This is a functional architectural model, not transistor-level reconstruction.

## Major blocks

1. Instruction register/fetch.
2. Instruction decoder.
3. Register file.
4. Effective-address generator.
5. Integer/logic execution.
6. Condition-code logic.
7. Memory/I/O interface.
8. Interrupt/trap controller.
9. Optional memory-management interface.
10. System-bus interface.
11. Clock/sequencing.

## Couplers

SLCoupler represents:

- fetch -> decoder;
- decoder -> register file;
- register file -> address generation;
- address generation -> bus;
- execution -> condition codes;
- bus -> memory/I/O;
- interrupt -> processor control;
- DMA -> bus arbitration.

## Executable circuit model

`SLPDP11Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP11CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **D** (4/10), with 2 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
