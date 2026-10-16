# AMD Ryzen Threadripper 9000 (Zen 5 x86-64) CPU

The SLeeLa AMD Ryzen Threadripper 9000 profile models the processor as a 64-bit many-core HEDT/workstation microprocessor with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | AMD Ryzen Threadripper 9000 ("Shimada Peak", Zen 5 x86-64) | documented |
| Data width | 64-bit | documented |
| External address width | 48-bit | documented |
| Address space | 256 TiB | derived from 48-bit addressing |
| Programmer registers | x86-64 register model (Zen 5 microarchitecture), per-core L1/L2, shared L3 per CCD | documented |
| Max cores / threads | 96 / 192 (PRO 9995WX) | documented |
| Rated clock (model) | about 5400 MHz max boost | documented/representative |
| Cache (model) | L1 + L2 + L3 (64–384 MB L3 across roster) | documented/architecture model |
| Process | 4 nm "Zen 5" | documented |
| Socket | sTR5 (TRX50 HEDT / WRX90 PRO) | documented |
| Memory | DDR5-6400 (4-channel HEDT, 8-channel PRO) | documented |
| PCIe | PCIe 5.0 (up to 88 lanes HEDT, up to 128 lanes PRO) | documented |
| TDP | 350 W | documented |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation. The full per-SKU roster (9980X/9970X/9960X and PRO 9995WX…9945WX) is tabulated in SPECIFICATION.md.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLAMDThreadripperCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

AMD Zen 5 high-end desktop / workstation line. Ryzen is the mainstream desktop sibling and EPYC is the server sibling (separate profiles). Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Zen_5
- https://en.wikipedia.org/wiki/Ryzen_Threadripper
- https://en.wikipedia.org/wiki/List_of_AMD_Ryzen_processors
