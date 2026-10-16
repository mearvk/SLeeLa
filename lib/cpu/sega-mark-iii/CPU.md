# Sega Mark III (Z80) CPU

The SLeeLa Sega Mark III (Z80) profile models the processor as a 8-bit console main CPU (Z80) with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Sega Mark III (Z80) | documented |
| Data width | 8-bit | documented |
| External address width | 16-bit | documented |
| Address space | 64 KiB | derived from 16-bit addressing |
| Programmer registers | Z80 register model (AF, BC, DE, HL + alternates, IX, IY, SP, PC) | documented |
| Rated clock (model) | about 4 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLMarkIIICPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Sega Mark III (Master System in Japan) main CPU is a Zilog Z80. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Sega_Mark_III
- https://en.wikipedia.org/wiki/Zilog_Z80
