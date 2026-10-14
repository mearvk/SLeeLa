# Xbox 360 (Xenon / PowerPC) CPU

The SLeeLa Xbox 360 (Xenon / PowerPC) profile models the processor as a 64-bit console main CPU (PowerPC tri-core) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Xbox 360 (Xenon / PowerPC) | documented |
| Data width | 64-bit | documented |
| External address width | 42-bit | documented |
| Address space | 4 TiB | derived from 42-bit addressing |
| Programmer registers | Power ISA register model (32 GP, 32 FP, VMX128) across three SMT2 cores; IBM Xenon | documented |
| Rated clock (model) | about 3200 MHz | documented/representative |
| Cache (model) | L1 + L2 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLXbox360CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Xbox 360 CPU is IBM Xenon, a triple-core PowerPC design (PPE-derived). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Xenon_(processor)
- https://en.wikipedia.org/wiki/Xbox_360_technical_specifications
