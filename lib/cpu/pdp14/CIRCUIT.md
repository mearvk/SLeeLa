# PDP-14 Logical Control Model

This is a logical control diagram specification, not a verified electrical schematic.

Conceptual blocks:
- control-program sequencer
- instruction decoder
- input image and sampling interface
- logic evaluation and internal state
- output image and commit interface
- fault and diagnostic handling

Shared SLeeLa logic gates and latches may be used for visualization where appropriate. Any inferred wiring must be labeled as derived; do not imply verified board-level circuitry.

## Executable circuit model

`SLPDP14Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP14CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
