# Atari Jaguar (68000 + Tom/Jerry RISC) CPU

The SLeeLa Atari Jaguar (68000 + Tom/Jerry RISC) profile models the processor as a 32/64-bit console (68000 + RISC) with a CISC named/split register set and an architectural, timing-oriented view of its execution.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Atari Jaguar (68000 + Tom/Jerry RISC) | documented |
| Data width | 32-bit | documented |
| External address width | 24-bit | documented |
| Address space | 16 MiB | derived from 24-bit addressing |
| Programmer registers | 68000 register model (D0-D7, A0-A7, PC, SR) as the manager CPU plus Tom/Jerry RISC cores | documented |
| Rated clock (model) | about 27 MHz | documented/representative |
| Cache (model) | None | documented/architecture model |
| Integrated DMA | Yes | documented/architecture model |
| MMU/TLB | No | documented/architecture model |

The profile is a functional model: enough register, memory and timing structure to run the CPU and host SLeeLa/C/C++ workloads, not a cycle-accurate silicon recreation.

## SLeeLa modeling

The profile composes the shared CPU objects:

- SLAtariJaguarCPU (extends SLCPURuntime)
- SLClock
- SLRegisterFile
- SLALU
- SLInstructionDecoder
- SLIOBus
- SLIOTiming
- SLInterruptController

Cache, DMA and MMU objects are instantiated only where this part actually integrates them (see CACHE.md, DMA.md).

## Variant rule

Atari Jaguar uses a Motorola 68000 manager plus custom Tom/Jerry RISC processors. Do not silently merge variant-specific I/O, cache, addressing or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.

## Sources

- https://en.wikipedia.org/wiki/Atari_Jaguar
- http://atarijaguar.freeservers.com/hardware.htm
