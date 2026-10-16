# Zilog eZ80 CPU

The SLeeLa Zilog eZ80 profile models the processor as a 8-bit microprocessor with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Zilog eZ80 | documented |
| Data width | 8-bit | documented |
| External address width | 24-bit | documented |
| Address space | 16 MiB | derived from 24-bit addressing |
| Programmer registers | Z80-compatible register set (AF, BC, DE, HL, IX, IY, SP, PC) with 24-bit addressing | documented |
| Rated clock (model) | about 50 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLeZ80CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Zilog eZ80, a pipelined Z80-compatible core addressing up to 16 MB without an MMU. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Zilog_eZ80
