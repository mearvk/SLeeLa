# Nintendo Switch 2 (ARM Cortex-A78C) CPU

The SLeeLa Nintendo Switch 2 (ARM Cortex-A78C) profile models the processor as a 64-bit console SoC (ARM64) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Nintendo Switch 2 (ARM Cortex-A78C) | documented |
| Data width | 64-bit | documented |
| External address width | 48-bit | documented |
| Address space | 256 TiB | derived from 48-bit addressing |
| Programmer registers | AArch64 register model; custom NVIDIA Tegra SoC with ARM Cortex-A78C-class cluster | documented |
| Rated clock (model) | about 1100 MHz | documented/representative |
| Cache (model) | L1 + L2 + L3 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLSwitch2CPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Nintendo Switch 2 SoC is a custom NVIDIA Tegra (ARM Cortex-A78C-class). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Nintendo_Switch_2
- https://en.wikipedia.org/wiki/ARM_Cortex-A78
