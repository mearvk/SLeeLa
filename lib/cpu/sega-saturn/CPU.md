# Sega Saturn (SH-2) CPU

The SLeeLa Sega Saturn (SH-2) profile models the processor as a 32-bit console main CPU (dual SH-2) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Sega Saturn (SH-2) | documented |
| Data width | 32-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | Hitachi SH-2 register model (R0-R15, PC, SR, GBR, VBR); two SH-2 cores in parallel | documented |
| Rated clock (model) | about 29 MHz | documented/representative |
| Cache (model) | L1 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLSaturnCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Sega Saturn uses two Hitachi SH-2 processors working in parallel. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://electronics.howstuffworks.com/dreamcast.htm
- https://en.wikipedia.org/wiki/SuperH
