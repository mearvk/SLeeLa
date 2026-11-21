# System/370 Logical Circuit Abstraction

## Scope

This describes logical simulation blocks, not a transistor-level IBM System/370 circuit design.

## Functional blocks

- Instruction fetch, decode, and execution control.
- General-purpose register file and condition-code logic.
- System/370 PSW and interruption-state control.
- Integer arithmetic, branch, and storage-operation units.
- Optional floating-point and control-state blocks.
- Optional DAT and protection-checking path.
- Storage interface and channel-I/O request interface.

## Configuration

Instantiate only facilities supported by the selected architecture profile. Keep machine-specific storage, channels, devices, timers, and cache structures outside the generic core unless evidence supports a particular integration.

## Limits

Do not infer gate counts, physical layout, clock frequency, power draw, or undocumented pipeline depth from this abstraction.

## Executable circuit model

`SLSystem370Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLSystem370CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
