# WDC 65C816 CPU

The SLeeLa WDC 65C816 profile models the processor as a 8/16-bit microprocessor with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | WDC 65C816 | documented |
| Data width | 16-bit | documented |
| External address width | 24-bit | documented |
| Address space | 16 MiB | derived from 24-bit addressing |
| Programmer registers | A (8/16), X/Y (8/16), 16-bit stack pointer, direct-page, 24-bit program address, status | documented |
| Rated clock (model) | about 4 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SL65C816CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

WDC 65C816, a 16-bit follow-up to the 65C02 with 24-bit addressing. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://wikiwand.com/en/articles/65C816
