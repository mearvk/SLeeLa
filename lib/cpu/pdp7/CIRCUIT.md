# PDP-7 Logical Circuit Model

This is a logical architecture model for diagnostics and teaching, not a verified transistor-level schematic.

Conceptual blocks:
- instruction register and PDP-7 decoder
- PC and memory address path
- 18-bit accumulator/arithmetic path
- memory interface
- control/sequencing logic
- I/O decoder and peripheral dispatch

SLeeLa gates, latches, flip-flops, adders, and bus abstractions may be used where appropriate. Mark inferred wiring as derived; do not present it as verified board-level detail.

## Executable circuit model

`SLPDP7Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP7CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
