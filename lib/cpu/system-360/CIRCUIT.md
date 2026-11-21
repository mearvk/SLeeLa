# System/360 Logical Circuit Abstraction

## Scope

This is a logical block diagram specification for simulation and education, not a transistor-level IBM System/360 netlist.

## Functional blocks

- Instruction fetch, decode, and execution control.
- General-purpose register file and condition-code state.
- PSW and interruption-state control.
- Integer arithmetic, branch, and storage-operation logic.
- Optional floating-point and other documented model features.
- Storage interface and protection checks.
- Channel/I/O request and interruption interfaces.

## Machine boundary

Storage controllers, channels, peripheral devices, timers, and implementation-specific buffering are system-level components unless a selected machine profile documents a particular integration.

## Validation

Instantiate only blocks justified by the selected model. Do not infer gate count, physical layout, clock rate, or power consumption from this logical abstraction.

## Executable circuit model

`SLSystem360Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLSystem360CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
