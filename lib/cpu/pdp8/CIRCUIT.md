# PDP-8 Logical Circuit Abstraction

## Scope

This is a logical educational/simulation abstraction, not a transistor-level DEC PDP-8 schematic.

## Functional blocks

- 12-bit instruction fetch and decode.
- 12-bit accumulator and separate link bit.
- 12-bit program counter and address selection.
- Memory-reference effective-address logic.
- Operate-instruction micro-operation control.
- Optional EAE arithmetic block.
- IOT dispatch and interrupt interface.
- Word-addressed memory interface.

## Variant control

Add memory-extension logic, arithmetic options, and device interfaces only when documented for the selected model. Do not infer gate counts, physical layout, power draw, or clock speed.

## Executable circuit model

`SLPDP8Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP8CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
