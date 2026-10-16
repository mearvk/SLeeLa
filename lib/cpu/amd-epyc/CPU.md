# AMD EPYC (Zen x86-64) CPU

The SLeeLa AMD EPYC (Zen x86-64) profile models the processor as a 64-bit server multi-core microprocessor with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | AMD EPYC (Zen x86-64) | documented |
| Data width | 64-bit | documented |
| External address width | 48-bit | documented |
| Address space | 256 TiB | derived from 48-bit addressing |
| Programmer registers | x86-64 register model (Zen) with chiplet CCDs, large shared L3 and ECC/RAS | documented |
| Max cores / threads | 192 / 384 (EPYC 9965 "Turin", Zen 5c) | documented |
| Rated clock (model) | about 3700 MHz family default; up to 5.0 GHz boost on F-series SKUs | documented/representative |
| Cache (model) | L1 + L2 + L3 (up to 512 MB L3) | documented/architecture model |
| Integrated DMA | No | documented/architecture model |
| MMU/TLB | Yes | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLAMDEPYCCPU (extends SLCPURuntime) — family base: decode(), ISA, Zen x86-64 programmer model
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

### Per-SKU classes

Individual EPYC 9005 "Turin" SKUs are thin subclasses of SLAMDEPYCCPU that override only configure() with the SKU's rated clock and identity label (via the shared bringUpModel() helper); they inherit decode() and the whole runnable substrate:

- SLAMDEPYC9965, SLAMDEPYC9755, SLAMDEPYC9645, SLAMDEPYC9575F, SLAMDEPYC9475F, SLAMDEPYC9355, SLAMDEPYC9175F

Each is selectable from SLCPUCatalog by its own key (epyc-9965, epyc-9755, …). See SPECIFICATION.md for the per-SKU table.

## Variant rule

Server/datacenter sibling of Ryzen using the same Zen microarchitecture. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Epyc
- https://en.wikipedia.org/wiki/Zen_(microarchitecture)
- https://en.wikipedia.org/wiki/Zen_5
- https://www.amd.com/en/products/processors/server/epyc/9005-series.html
