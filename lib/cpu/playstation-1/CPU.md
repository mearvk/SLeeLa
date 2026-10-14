# PlayStation (MIPS R3000A) CPU

The SLeeLa PlayStation (MIPS R3000A) profile models the processor as a 32-bit console main CPU (MIPS) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | PlayStation (MIPS R3000A) | documented |
| Data width | 32-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | MIPS I register model (32 GP, r0 hardwired zero, HI/LO); MIPS R3000A-class | documented |
| Rated clock (model) | about 34 MHz | documented/representative |
| Cache (model) | L1 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLPlayStation1CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

PlayStation main CPU is a MIPS R3000A-compatible core (LSI CoreWare). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/PlayStation_technical_specifications
- https://www.psdevwiki.com/ps1/CPU
