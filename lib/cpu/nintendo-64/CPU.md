# Nintendo 64 (NEC VR4300 / MIPS) CPU

The SLeeLa Nintendo 64 (NEC VR4300 / MIPS) profile models the processor as a 64-bit console main CPU (MIPS) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Nintendo 64 (NEC VR4300 / MIPS) | documented |
| Data width | 64-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | MIPS III register model (32 GP, r0 hardwired zero, HI/LO, 32 FP); NEC VR4300 | documented |
| Rated clock (model) | about 94 MHz | documented/representative |
| Cache (model) | L1 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLNintendo64CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Nintendo 64 main CPU is a NEC VR4300 (MIPS R4300i-class). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Nintendo_64
- https://en.wikipedia.org/wiki/NEC_VR4300
