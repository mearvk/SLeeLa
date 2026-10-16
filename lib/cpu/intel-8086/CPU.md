# Intel 8086 CPU

The SLeeLa Intel 8086 profile models the processor as a 16-bit microprocessor with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Intel 8086 | documented |
| Data width | 16-bit | documented |
| External address width | 20-bit | documented |
| Address space | 1 MiB | derived from 20-bit addressing |
| Programmer registers | AX, BX, CX, DX, SI, DI, BP, SP, IP, segment registers (CS/DS/SS/ES), FLAGS | documented |
| Rated clock (model) | about 5 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLIntel8086CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Intel 8086/8088 is the origin of the x86 ISA. 80186/80286 are separate profiles. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/X86
- https://en.wikipedia.org/wiki/Intel_8086
