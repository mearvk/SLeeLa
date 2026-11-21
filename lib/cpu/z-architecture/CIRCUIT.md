# z/Architecture Logical Circuit Abstraction

## Scope

This document describes logical blocks for simulation and education; it is not a physical IBM processor netlist.

## Functional blocks

- Versioned instruction fetch/decode and execution control.
- General-purpose register file and condition-code state.
- PSW and privileged control-state management.
- Arithmetic, branch, and storage-operation units.
- Optional floating-point, vector, decimal and other facility blocks.
- Translation/protection interface to the system model.
- Interruption interface and optional performance abstraction.

## System boundary

Main storage controllers, cache hierarchy details, channel subsystem, I/O devices, multiprocessor interconnect and firmware are system-level components unless the selected target model explicitly integrates a defined part.

## Validation

Instantiate only facilities enabled for the selected profile. Do not infer transistor count, circuit topology, clock rate or power consumption from this logical model.

## Executable circuit model

`SLZArchitectureCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLZArchitectureCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
