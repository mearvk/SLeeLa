# SLeeLa CPU Specification

## Purpose

This is the master specification for detailed CPU modeling under `/lib/cpu`.

Every CPU or console processor descriptor should evolve from its compact `CPU.md` into a detailed `SPECIFICATION.md` when sufficient authoritative information is available.

## Required specification domains

| Domain | Required information |
|---|---|
| Identity | manufacturer, model, family, revision |
| Process | process node/package when documented |
| Architecture | ISA, width, modes, privilege |
| Clock | frequency, domains, multiplier/divider where documented |
| Pipeline | stages and throughput/latency where documented |
| Registers | architectural/control/status registers |
| Execution | ALU, FPU, SIMD/vector, special units |
| Cache | L1 I/D, L2, L3 and coherence |
| Memory | controller, width, timing and address model |
| I/O | buses, devices, transaction width |
| Timing | bus cycles, wait states, I/O latency |
| DMA | channels, descriptors, arbitration |
| Interrupts | sources, priority and entry behavior |
| MMU/TLB | translation and protection |
| Interconnect | buses/fabric/couplers |
| Power | states and clock gating when documented |

## Evidence discipline

Use manufacturer manuals, programmer's references, architecture manuals, datasheets and reputable technical references. Cite the source in the per-CPU specification.

Do not turn an approximation into a hardware fact. Use `documented`, `derived`, `estimated`, or `unspecified` labels where appropriate.

## Iterative implementation

Phase 1: capture facts in `SPECIFICATION.md`.

Phase 2: connect the profile to the common SLeeLa CPU objects.

Phase 3: add timing, bus, cache, DMA and register behavior.

Phase 4: add validation tests.

Phase 5: add deeper circuit-level descriptions only where reliable public documentation exists.

The objective is a reusable architectural model, not a claim that the source code reproduces the vendor's transistor-level implementation.