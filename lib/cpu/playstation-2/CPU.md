# PlayStation 2 (Emotion Engine / MIPS) CPU

The SLeeLa PlayStation 2 (Emotion Engine / MIPS) profile models the processor as a 64-bit console main CPU (MIPS) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | PlayStation 2 (Emotion Engine / MIPS) | documented |
| Data width | 64-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | MIPS III/IV register model (32 128-bit GP, r0 zero) in the Emotion Engine | documented |
| Rated clock (model) | about 294 MHz | documented/representative |
| Cache (model) | L1 + L2 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLPlayStation2CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

PlayStation 2 main CPU is the Emotion Engine (MIPS R5900-based). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Emotion_Engine
- https://en.wikipedia.org/wiki/Comparison_of_PlayStation_consoles
