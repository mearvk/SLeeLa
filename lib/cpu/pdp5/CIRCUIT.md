# PDP-5 Circuit Model

The SLeeLa circuit view is a logical teaching/diagnostic model, not a transistor-level reconstruction.

Conceptual blocks:
- instruction register and decoder
- program counter and address path
- accumulator and link arithmetic path
- core-memory interface
- I/O instruction and device dispatch
- sequencing/control logic

Use shared SLeeLa gate, latch, flip-flop, adder, and bus abstractions only when useful to the selected fidelity level. Label inferred wiring as derived; do not imply verified board-level schematics.

## Executable circuit model

`SLPDP5Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP5CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
