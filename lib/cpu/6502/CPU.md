# MOS Technology 6502 CPU

The SLeeLa 6502 profile models the original MOS Technology 6502 as an architectural and timing-oriented 8-bit processor.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | MOS 6502 | documented |
| Data width | 8-bit | documented |
| External address width | 16-bit | documented |
| Address space | 64 KiB | derived from 16-bit addressing |
| Programmer registers | A, X, Y, S, PC, P | documented |
| Typical original clock | about 1 MHz | documented/common original implementation |
| Pipeline | No modern multi-stage pipeline | derived from cycle-level architecture |
| L2/L3 cache | None | documented/architecture model |
| Integrated DMA | None in the base CPU | documented; external hardware may perform DMA |
| MMU/TLB | None in the base 6502 | documented |

The 6502 is modeled as a small, bus-visible processor rather than as a modern superscalar CPU. Its observable behavior is strongly tied to individual bus cycles and the two-phase clock.

## SLeeLa modeling

The profile composes:

- SL6502CPU
- SLClock
- SLRegisterFile
- SLPipeline (cycle sequencer, not a modern pipeline)
- SLInstructionFetch
- SLExecutionUnit
- SLIOBus
- SLIOTiming
- SLInterruptController
- SLCoupler

The common cache, DMA and MMU objects remain explicitly disabled/absent for the base 6502.

## Variant rule

6502-derived parts such as the 6510, 6504, 6509, RP2A03 and 65C02 should be separate profiles. Do not silently merge variant-specific I/O, banking, decimal-mode, CMOS, or opcode behavior into this base model.

See SPECIFICATION.md for the detailed model.
