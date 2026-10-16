# Atari 2600 (6507 / 6502) CPU

The SLeeLa Atari 2600 (6507 / 6502) profile models the processor as a 8-bit console main CPU (6507) with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Atari 2600 (6507 / 6502) | documented |
| Data width | 8-bit | documented |
| External address width | 13-bit | documented |
| Address space | 8 KiB | derived from 13-bit addressing |
| Programmer registers | 6502-class model (A, X, Y, S, PC, P); MOS 6507 with a 13-bit address bus | documented |
| Rated clock (model) | about 1 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLAtari2600CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Atari 2600 CPU is a MOS 6507, a reduced-pin 6502 with 8 KiB addressing. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Atari_2600_hardware
- https://en.wikipedia.org/wiki/MOS_Technology_6507
