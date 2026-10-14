# Sega Pico (68000) CPU

The SLeeLa Sega Pico (68000) profile models the processor as a 16/32-bit edutainment main CPU (68000) with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Sega Pico (68000) | documented |
| Data width | 32-bit | documented |
| External address width | 24-bit | documented |
| Address space | 16 MiB | derived from 24-bit addressing |
| Programmer registers | 68000 register model (D0-D7, A0-A7, PC, SR) | documented |
| Rated clock (model) | about 8 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLPicoCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Sega Pico is Genesis-class hardware; main CPU is a Motorola 68000. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Sega_Pico
- https://en.wikipedia.org/wiki/Sega_Genesis
