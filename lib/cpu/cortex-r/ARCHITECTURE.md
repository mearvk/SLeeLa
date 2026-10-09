# Cortex-R Architecture

## Execution and instruction sets

Cortex-R spans multiple architecture generations and implementation profiles. Instruction-set state, instruction encodings, exception model, and system registers must be selected from the target's documented architecture. Do not assume every Cortex-R supports the same ARM/Thumb features or that all share one execution model.

## Real-time design concerns

The model must make memory latency and interrupt behavior explicit. Tightly coupled memories can provide predictable access for configured regions; caches can improve average performance but introduce hit/miss variability. Bus arbitration and external memory wait states remain platform concerns.

## Exceptions and interrupts

Implement exception modes, status preservation, vectoring, masking, and return semantics according to the selected architecture generation. Interrupt-controller details and interrupt routing may be integrated, external, or platform-specific; configure them rather than assuming a single universal controller.

## Memory protection and translation

Support the selected core's protection and translation facilities only when present. MPU and MMU behavior are not interchangeable. Represent region attributes, access permissions, privilege, execute permissions, and faults according to the chosen profile.

## Optional execution features

FPU, DSP, virtualization, debug/trace, and safety-related facilities vary by implementation. Each requires an explicit capability flag and feature-specific decode/state behavior.

## SoC boundary

DMA, timers, watchdogs, flash controllers, safety islands, interconnects, and external memory devices are modeled as platform components unless the selected implementation documents a specific integrated block.
