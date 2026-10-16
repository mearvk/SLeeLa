# NVIDIA Grace (ARM Neoverse V2) CPU

The SLeeLa NVIDIA Grace (ARM Neoverse V2) profile models the processor as a 64-bit server CPU (ARM Neoverse) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | NVIDIA Grace (ARM Neoverse V2) | documented |
| Data width | 64-bit | documented |
| External address width | 48-bit | documented |
| Address space | 256 TiB | derived from 48-bit addressing |
| Programmer registers | AArch64 register model; many Neoverse-V2 cores with a coherent mesh and large shared cache | documented |
| Rated clock (model) | about 3400 MHz | documented/representative |
| Cache (model) | L1 + L2 + L3 | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLNvidiaGraceCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

NVIDIA Grace datacenter CPU built on ARM Neoverse V2 cores. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://www.nvidia.com/en-us/data-center/grace-cpu/
- https://en.wikipedia.org/wiki/ARM_Neoverse
