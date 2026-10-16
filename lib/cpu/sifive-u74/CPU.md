# SiFive U74 (RISC-V RV64GC) CPU

The SLeeLa SiFive U74 (RISC-V RV64GC) profile models the processor as a 64-bit application core (RISC-V RV64GC) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | SiFive U74 (RISC-V RV64GC) | documented |
| Data width | 64-bit | documented |
| External address width | 39-bit | documented |
| Address space | 512 GiB | derived from 39-bit addressing |
| Programmer registers | RISC-V RV64GC register model (x0 hardwired zero, x1-x31, 32 FP), Sv39 MMU | documented |
| Rated clock (model) | about 1500 MHz | documented/representative |
| Cache (model) | L1 + L2 | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLSiFiveU74CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

SiFive U74 application core (RV64GC) used in Linux-capable RISC-V SoCs. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://www.scs.stanford.edu/~zyedidia/docs/sifive/sifive-u74.pdf
- https://en.wikipedia.org/wiki/RISC-V
