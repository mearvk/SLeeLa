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
