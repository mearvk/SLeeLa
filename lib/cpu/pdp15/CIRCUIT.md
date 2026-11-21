# PDP-15 Logical Circuit Model

The SLeeLa circuit representation is a conceptual architecture model, not a verified transistor-level reconstruction.

Conceptual blocks:
- PDP-15 instruction register and decoder
- program counter and address path
- 18-bit accumulator and arithmetic/logical unit
- memory interface
- sequencing and control-flow logic
- I/O dispatch and configured peripheral interfaces
- option-dependent execution units when explicitly enabled

Use shared SLeeLa gates, latches, flip-flops, adders, and bus abstractions where useful. Mark inferred structure as derived; do not present it as a verified board schematic.

## Executable circuit model

`SLPDP15Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP15CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
