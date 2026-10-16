# Intel 80486 CPU

The SLeeLa Intel 80486 profile models the processor as a 32-bit pipelined microprocessor with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Intel 80486 | documented |
| Data width | 32-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | IA-32 register model (EAX..ESP, EFLAGS, EIP, control registers) with integrated FPU | documented |
| Rated clock (model) | about 66 MHz | documented/representative |
| Cache (model) | L1 | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLIntel80486CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

First tightly pipelined x86 with on-chip L1 cache and integrated FPU. Pentium is a separate profile. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/I486
