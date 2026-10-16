# Sega Dreamcast (SH-4) CPU

The SLeeLa Sega Dreamcast (SH-4) profile models the processor as a 32-bit console main CPU (SH-4) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Sega Dreamcast (SH-4) | documented |
| Data width | 32-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | Hitachi SH-4 register model (R0-R15 banked, PC, SR, FP bank) with on-chip cache and MMU | documented |
| Rated clock (model) | about 200 MHz | documented/representative |
| Cache (model) | L1 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLDreamcastCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Sega Dreamcast main CPU is a Hitachi SH-4. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Dreamcast
- https://en.wikipedia.org/wiki/SuperH
