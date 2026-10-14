# Atari Lynx (65C02 / Mikey) CPU

The SLeeLa Atari Lynx (65C02 / Mikey) profile models the processor as a 8-bit handheld main CPU (65C02) with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Atari Lynx (65C02 / Mikey) | documented |
| Data width | 8-bit | documented |
| External address width | 16-bit | documented |
| Address space | 64 KiB | derived from 16-bit addressing |
| Programmer registers | 65C02 programmer model (A, X, Y, S, PC, P) with the Mikey/Suzy custom chips | documented |
| Rated clock (model) | about 4 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLAtariLynxCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Atari Lynx CPU is a 4 MHz WDC 65C02 paired with custom blitter hardware. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Atari_Lynx
- https://en.wikipedia.org/wiki/WDC_65C02
