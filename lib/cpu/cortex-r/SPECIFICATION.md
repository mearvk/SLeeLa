# Cortex-R Specification and Profile Contract

## Required configuration

- `core_variant`: exact Cortex-R implementation or explicitly named architecture profile.
- `instruction_profile`: instruction-set generation and enabled instruction extensions.
- `memory_map`: platform-defined address regions and attributes.
- `memory_system`: cache, TCM, wait states, bus and memory-controller configuration.
- `interrupt_model`: supported interrupt controller, sources, priorities and routing.
- `extensions`: explicit FPU/DSP, MPU, debug/trace, virtualization, and safety-related options.
- `strict_profile`: defaults to true.

## Architectural state

The register model is determined by the selected architecture generation and execution state. It may include general-purpose registers, PC, link register, status registers, banked registers for exception modes, and system-control registers. Do not assume one universal banked-register layout for all Cortex-R generations.

Optional FPU registers, virtualization state, debug registers, and implementation-specific registers are exposed only when the selected core provides them.

## Required behavior

1. Decode only instructions valid for the selected architecture profile and enabled extensions.
2. Preserve architectural condition and status flags.
3. Model exception entry/return, mode changes, banked state and interrupt masking according to the selected generation.
4. Apply memory protection, access permissions, alignment and fault behavior defined for that profile.
5. Keep SoC-specific memory maps and peripherals outside the generic CPU definition.
6. Identify implementation-defined or unknown behavior explicitly.

## Memory-system options

Caches, tightly coupled memories, MPU/MMU-like facilities, and memory attributes vary by core and configuration. Enable each only when documented. TCM regions must be represented as configured memory regions with target-specific latency and access rules.

## Real-time evidence

Worst-case execution time is not a generic property of the family. Reports must identify core, memory placement, cache/TCM settings, interrupt assumptions, bus contention assumptions, and evidence quality.

## Error handling

Invalid profile combinations fail configuration. Unsupported opcodes, inaccessible registers, permission failures, and illegal state transitions produce modeled architectural exceptions or structured diagnostics as appropriate; they never silently execute as another operation.
