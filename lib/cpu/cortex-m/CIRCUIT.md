# Cortex-M Circuit-Level Abstraction

## Scope

This document defines a logical digital abstraction for simulation and teaching. It is not a transistor-level netlist or a claim about a specific vendor's silicon.

## Functional blocks

- Thumb instruction fetch/decode and execution control.
- General-purpose register bank and status/exception state.
- Arithmetic/logic and optional DSP/FPU/MVE units.
- Exception interface and NVIC-facing signals.
- Configurable memory interface, optional MPU and cache blocks.
- Optional debug/trace, security attribution, and implementation-defined blocks.

## Signal-level concepts

Represent clock/reset, instruction/data request and response, exception/interrupt inputs, fault outputs, and bus handshakes at the abstraction level needed by the simulator. Exact signal names and timing are implementation-specific.

## Optionality

FPU, DSP, MVE, TrustZone-M, MPU, caches, and debug/trace must be conditionally instantiated. DMA and most peripherals belong to the SoC diagram, not the core schematic.

## Validation

Circuit abstraction must remain consistent with the selected architecture profile. Do not infer gate count, transistor count, physical timing, or power figures without a documented implementation source.

## Executable circuit model

`SLCortexMCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLCortexMCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
