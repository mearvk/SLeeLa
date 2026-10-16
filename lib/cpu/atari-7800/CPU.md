# Atari 7800 (6502C) CPU

The SLeeLa Atari 7800 (6502C) profile models the processor as a 8-bit console main CPU (6502C) with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Atari 7800 (6502C) | documented |
| Data width | 8-bit | documented |
| External address width | 16-bit | documented |
| Address space | 64 KiB | derived from 16-bit addressing |
| Programmer registers | 6502-class model (A, X, Y, S, PC, P); MOS 6502C (SALLY) | documented |
| Rated clock (model) | about 2 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLAtari7800CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Atari 7800 CPU is a MOS 6502C (SALLY). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Atari_7800
- https://en.wikipedia.org/wiki/MOS_Technology_6502
