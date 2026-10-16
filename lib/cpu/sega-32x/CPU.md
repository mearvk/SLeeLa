# Sega 32X (SH-2) CPU

The SLeeLa Sega 32X (SH-2) profile models the processor as a 32-bit add-on CPU (dual SH-2) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Sega 32X (SH-2) | documented |
| Data width | 32-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | Hitachi SH-2 register model (R0-R15, PC, SR, GBR, VBR); two SH-2 cores | documented |
| Rated clock (model) | about 23 MHz | documented/representative |
| Cache (model) | L1 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLSega32XCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Sega 32X adds two Hitachi SH-2 processors to the Genesis. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/32X
- https://treasure.fandom.com/wiki/Sega_32X/Technical_Specifications
