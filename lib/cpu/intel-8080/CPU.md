# Intel 8080 CPU

The SLeeLa Intel 8080 profile models the processor as a 8-bit microprocessor with a accumulator / indexed register machine and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Intel 8080 | documented |
| Data width | 8-bit | documented |
| External address width | 16-bit | documented |
| Address space | 64 KiB | derived from 16-bit addressing |
| Programmer registers | A, B, C, D, E, H, L accumulator/register file, SP, PC, flags | documented |
| Rated clock (model) | about 2 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLIntel8080CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Intel 8080 -> 8085 -> 8086 (x86). Derivatives (8085, Z80) are separate profiles. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Intel_8080
- https://en.wikipedia.org/wiki/X86
