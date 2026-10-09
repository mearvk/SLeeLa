# Cortex-R Circuit-Level Abstraction

## Scope

This is a logical block-level representation for SLeeLa simulation and teaching, not a transistor-level netlist.

## Functional blocks

- Profile-specific instruction fetch, decode and execution.
- Register banks and status/control state.
- Arithmetic/logic and optional FPU/DSP units.
- Exception and interrupt interface.
- Memory protection/translation block where implemented.
- Optional cache and TCM interfaces.
- Bus and memory-system interfaces.
- Optional debug/trace, virtualization and safety-related interfaces.

## Signal groups

At the abstraction level needed by the simulator, represent clock/reset, instruction/data requests and responses, interrupt inputs, exception/fault outputs, memory attributes, and barrier/order control. Exact signal names and cycle timing are implementation-specific.

## Optional blocks

Do not instantiate caches, TCM, FPU, MPU/MMU, virtualization, or safety monitors without target evidence. External DMA and peripherals belong in the SoC-level circuit.

## Validation

Ensure every block corresponds to the selected profile and configuration. Do not infer transistor counts, physical timing, power, or gate-level topology without a documented implementation source.
