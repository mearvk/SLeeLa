# Intel Pentium CPU

The SLeeLa Intel Pentium profile models the processor as a 32-bit superscalar microprocessor with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Intel Pentium | documented |
| Data width | 32-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | IA-32 register model with two integer pipelines (U/V) and split code/data L1 caches | documented |
| Rated clock (model) | about 100 MHz | documented/representative |
| Cache (model) | L1 | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLIntelPentiumCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Superscalar extension of the 80486 with a 64-bit external data bus. Later P6 cores are separate profiles. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Pentium
