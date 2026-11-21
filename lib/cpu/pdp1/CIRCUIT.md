# PDP-1 Logical Circuit Model

This is a logical teaching and diagnostic model, not a verified transistor-level reconstruction.

Conceptual blocks:
- instruction register and PDP-1 decoder
- program counter and address path
- 18-bit accumulator/arithmetic path
- memory interface
- sequencing and control-flow logic
- console and peripheral I/O dispatch

Shared SLeeLa gates, latches, flip-flops, adders, and bus abstractions may be used where appropriate. Mark inferred connections as derived and do not present them as verified schematics.

## Executable circuit model

`SLPDP1Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP1CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
