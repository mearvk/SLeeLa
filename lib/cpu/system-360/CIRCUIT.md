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
