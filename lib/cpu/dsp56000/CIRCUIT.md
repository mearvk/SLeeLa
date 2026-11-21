# DSP56000 Functional Circuit Model

Functional model, not a transistor-level netlist. Blocks include program fetch/decode, register file, address-generation unit, fixed-point data ALU, multiplier-accumulator, hardware-loop controller, program-memory interface, X/Y data-memory interfaces, interrupt logic, and clock sequencing. Add peripherals or DMA only when the selected device/system profile supports them. SLCoupler represents operand delivery, parallel data movement, MAC writeback, memory traffic, loop control, and interrupt transitions.

## Executable circuit model

`SLDSP56000Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLDSP56000CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
