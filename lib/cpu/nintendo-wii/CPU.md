# Nintendo Wii (Broadway / PowerPC) CPU

The SLeeLa Nintendo Wii (Broadway / PowerPC) profile models the processor as a 32-bit console main CPU (PowerPC) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Nintendo Wii (Broadway / PowerPC) | documented |
| Data width | 32-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | PowerPC 750-class register model (32 GP, 32 FP) with on-chip L1/L2; IBM Broadway | documented |
| Rated clock (model) | about 729 MHz | documented/representative |
| Cache (model) | L1 + L2 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLWiiCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Wii main CPU is IBM Broadway, a higher-clocked evolution of the GameCube Gekko. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Broadway_(processor)
- https://en.wikipedia.org/wiki/PowerPC_7xx
