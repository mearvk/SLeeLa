# PDP-12 Logical Circuit Model

This is a logical teaching and diagnostics view, not a transistor-level schematic.

Conceptual blocks:
- shared memory and address interface
- mode selector / instruction-environment control
- PDP-8-family instruction decoder
- LINC-compatible instruction decoder
- environment-specific execution and register state
- I/O dispatch and sequencing logic

Use shared SLeeLa gates, latches, flip-flops, adders, and buses where they help represent behavior. Mark inferred connections as derived; do not imply a verified board-level reconstruction.

## Executable circuit model

`SLPDP12Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP12CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
