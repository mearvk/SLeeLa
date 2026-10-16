# Intel 80386 CPU

The SLeeLa Intel 80386 profile models the processor as a 32-bit microprocessor with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Intel 80386 | documented |
| Data width | 32-bit | documented |
| External address width | 32-bit | documented |
| Address space | 4 GiB | derived from 32-bit addressing |
| Programmer registers | EAX..ESP 32-bit general registers, segment registers, EFLAGS, EIP, CR0-CR3, paging MMU | documented |
| Rated clock (model) | about 33 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLIntel80386CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

80386 (IA-32) adds 32-bit registers and paging. The 80486 integrates cache/FPU and is a separate profile. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/I386
- https://en.wikipedia.org/wiki/X86
