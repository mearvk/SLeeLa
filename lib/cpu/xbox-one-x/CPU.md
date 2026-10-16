# Xbox One X (x86-64 Jaguar) CPU

The SLeeLa Xbox One X (x86-64 Jaguar) profile models the processor as a 64-bit console APU (x86-64 Jaguar) with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Xbox One X (x86-64 Jaguar) | documented |
| Data width | 64-bit | documented |
| External address width | 48-bit | documented |
| Address space | 256 TiB | derived from 48-bit addressing |
| Programmer registers | x86-64 register model; 8-core AMD Jaguar-evolved cluster at a higher clock | documented |
| Rated clock (model) | about 2300 MHz | documented/representative |
| Cache (model) | L1 + L2 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLXboxOneXCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Xbox One X CPU is an 8-core AMD Jaguar-evolved design (x86-64). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Xbox_One_X
- https://en.wikipedia.org/wiki/Jaguar_(microarchitecture)
