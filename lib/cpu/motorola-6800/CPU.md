# Motorola 6800 CPU

The SLeeLa Motorola 6800 profile models the processor as a 8-bit microprocessor with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Motorola 6800 | documented |
| Data width | 8-bit | documented |
| External address width | 16-bit | documented |
| Address space | 64 KiB | derived from 16-bit addressing |
| Programmer registers | A, B accumulators, X index, SP, PC, condition-code register | documented |
| Rated clock (model) | about 1 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SL6800CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Motorola 6800 (origin of the 68xx line). The 6809 is a separate profile. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Motorola_6800
