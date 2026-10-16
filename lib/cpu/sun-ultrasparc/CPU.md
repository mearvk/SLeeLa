# Sun UltraSPARC CPU

The SLeeLa Sun UltraSPARC profile models the processor as a 64-bit workstation/server CPU (SPARC V9) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Sun UltraSPARC | documented |
| Data width | 64-bit | documented |
| External address width | 44-bit | documented |
| Address space | 16 TiB | derived from 44-bit addressing |
| Programmer registers | SPARC V9 register windows (global + windowed in/out/local), g0 hardwired zero | documented |
| Rated clock (model) | about 1200 MHz | documented/representative |
| Cache (model) | L1 + L2 | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLSunUltraSPARCCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Sun UltraSPARC (SPARC V9) workstation/server line. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/UltraSPARC
- https://en.wikipedia.org/wiki/SPARC
