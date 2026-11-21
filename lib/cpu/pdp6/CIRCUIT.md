# PDP-6 Logical Circuit Model

This is a logical architecture view for SLeeLa diagnostics and education, not a verified transistor-level reconstruction.

Conceptual blocks:
- instruction register and PDP-6 decoder
- 36-bit register and arithmetic paths
- program control and effective-address logic
- memory interface
- I/O instruction and peripheral dispatch
- sequencing and exception/diagnostic path

Use shared SLeeLa gate, latch, flip-flop, adder, and bus abstractions where useful. Mark inferred relationships as derived; do not present them as verified board schematics.

## Executable circuit model

`SLPDP6Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP6CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
