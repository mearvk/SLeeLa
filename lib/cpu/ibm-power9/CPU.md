# IBM POWER9 CPU

The SLeeLa IBM POWER9 profile models the processor as a 64-bit server CPU (Power ISA) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | IBM POWER9 | documented |
| Data width | 64-bit | documented |
| External address width | 52-bit | documented |
| Address space | 4 PiB | derived from 52-bit addressing |
| Programmer registers | Power ISA v3.0 register model (32 GP, 32 VSR), SMT4/SMT8, large on-chip L2/L3 | documented |
| Rated clock (model) | about 3800 MHz | documented/representative |
| Cache (model) | L1 + L2 + L3 | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLIBMPower9CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

IBM POWER9 (Power ISA v3.0). POWER10 is the successor (separate profile). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/POWER9
- https://en.wikipedia.org/wiki/Power_ISA
