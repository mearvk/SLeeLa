# Nintendo SNES (65C816) CPU

The SLeeLa Nintendo SNES (65C816) profile models the processor as a 16-bit console main CPU with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Nintendo SNES (65C816) | documented |
| Data width | 16-bit | documented |
| External address width | 24-bit | documented |
| Address space | 16 MiB | derived from 24-bit addressing |
| Programmer registers | 65C816 programmer model (A, X, Y 8/16-bit, 16-bit SP, direct page, 24-bit PBR/addr) | documented |
| Rated clock (model) | about 4 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLSNESCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Nintendo SNES main CPU is a Ricoh 5A22 based on the WDC 65C816. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Ricoh_5A22
- https://snes.nesdev.org/wiki/Timing
