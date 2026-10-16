# Rockchip (ARM64) CPU

The SLeeLa Rockchip (ARM64) profile models the processor as a 64-bit embedded SoC (ARM64) with a RISC general-purpose register file and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Rockchip (ARM64) | documented |
| Data width | 64-bit | documented |
| External address width | 40-bit | documented |
| Address space | 1 TiB | derived from 40-bit addressing |
| Programmer registers | AArch64 register model; ARM Cortex-A big.LITTLE clusters in an application SoC | documented |
| Rated clock (model) | about 2400 MHz | documented/representative |
| Cache (model) | L1 + L2 | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLRockchipCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Rockchip ARM application SoCs (e.g. RK3399-class) built on licensed Cortex-A cores. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Rockchip
- https://en.wikipedia.org/wiki/AArch64
