# LINC-8 Logical Circuit Model

This is a logical educational/diagnostic model, not a verified transistor-level reconstruction.

Conceptual blocks:
- active-mode selector
- LINC instruction decoder and execution path
- PDP-8 instruction decoder and execution path
- shared memory interface
- mode-specific control and I/O dispatch
- machine-control and sequencing logic

Keep both decoders independently testable. Label inferred wiring as derived, and do not present a conceptual block diagram as an original DEC schematic.

## Executable circuit model

`SLLINC8Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLLINC8CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
