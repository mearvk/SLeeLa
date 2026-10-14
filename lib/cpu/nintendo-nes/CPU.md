# Nintendo NES (Ricoh 2A03 / 6502) CPU

The SLeeLa Nintendo NES (Ricoh 2A03 / 6502) profile models the processor as a 8-bit console main CPU with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Nintendo NES (Ricoh 2A03 / 6502) | documented |
| Data width | 8-bit | documented |
| External address width | 16-bit | documented |
| Address space | 64 KiB | derived from 16-bit addressing |
| Programmer registers | 6502-class programmer model (A, X, Y, S, PC, P); Ricoh 2A03 integrates audio and DMA | documented |
| Rated clock (model) | about 2 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLNESCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Nintendo NES main CPU is a Ricoh 2A03 (6502 core without decimal mode). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Ricoh_2A03
- https://www.nesdev.org/wiki/CPU
