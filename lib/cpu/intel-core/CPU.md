# Intel Core (x86-64) CPU

The SLeeLa Intel Core (x86-64) profile models the processor as a 64-bit multi-core microprocessor with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Intel Core (x86-64) | documented |
| Data width | 64-bit | documented |
| External address width | 48-bit | documented |
| Address space | 256 TiB | derived from 48-bit addressing |
| Programmer registers | x86-64 register model (16 GP, 16 SIMD) with per-core L1/L2 and shared L3 | documented |
| Rated clock (model) | about 3600 MHz | documented/representative |
| Cache (model) | L1 + L2 + L3 | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLIntelCoreCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Intel Core (x86-64) multi-core line. Xeon shares the microarchitecture at server scale (separate profile). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Intel_Core_(microarchitecture)
- https://en.wikipedia.org/wiki/X86-64
