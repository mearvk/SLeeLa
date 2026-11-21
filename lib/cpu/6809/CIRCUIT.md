# Motorola 6809 Circuit Model

## Functional scope

Architectural functional model, not transistor-level reconstruction.

## Blocks

1. Instruction fetch and prefix handling.
2. Opcode decoder.
3. A/B accumulators and D register view.
4. X/Y index registers.
5. S/U stack pointers and PC.
6. Direct-page and effective-address logic.
7. Integer/logic ALU.
8. Condition-code and interrupt control.
9. Memory/bus interface.
10. Clock and sequencing.

## Couplers

SLCoupler models fetch-to-decode, decode-to-registers, index/DP-to-address generation, ALU-to-condition codes, stack state-to-interrupt entry, and bus-to-memory paths.

## Executable circuit model

`SL6809Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SL6809CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **B** (8/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
