# Intel Celeron CPU

The SLeeLa Intel Celeron profile models the processor as a 32/64-bit value microprocessor with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Intel Celeron | documented |
| Data width | 32-bit | documented |
| External address width | 36-bit | documented |
| Address space | 64 GiB | derived from 36-bit addressing |
| Programmer registers | IA-32/x86-64 register model with reduced cache versus the contemporary Core/Pentium parts | documented |
| Rated clock (model) | about 1200 MHz | documented/representative |
| Cache (model) | L1 + L2 | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLIntelCeleronCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Value-segment derivative sharing the contemporary Intel core with reduced cache/features. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Celeron
