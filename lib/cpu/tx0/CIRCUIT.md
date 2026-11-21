# TX-0 Logical Circuit Model

This is a logical educational model, not a verified transistor-level reconstruction.

Conceptual blocks:
- instruction fetch and TX-0-specific decoder
- program sequencing
- arithmetic and accumulator datapath
- memory interface
- console and peripheral control
- instruction sequencing and status

Shared SLeeLa gates, latches, flip-flops, adders, and buses may be used where they help explain the datapath. Mark inferred connections as derived and avoid presenting them as original schematics.

## Executable circuit model

`SLTX0Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLTX0CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
