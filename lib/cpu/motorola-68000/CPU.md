# Motorola 68000 CPU

SLeeLa's Motorola 68000 profile models the original M68000 as a 32-bit programmer-visible architecture with a 16-bit external data bus and 24-bit address space.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Motorola 68000 / M68000 | documented |
| Programmer data width | 32-bit | documented |
| External data bus | 16-bit | documented |
| Address width | 24-bit | documented |
| Direct address space | 16 MiB | derived |
| Data registers | D0-D7, 32-bit | documented |
| Address registers | A0-A7, 32-bit | documented |
| PC | 32-bit | documented |
| Status | SR | documented |
| Cache | None in original MC68000 | architecture |
| Integrated DMA | None in original CPU | architecture; external peripherals may provide DMA |
| MMU/TLB | None in original MC68000 | architecture |

The M68000 is modeled as a bus-visible processor. Its external read/write cycles, asynchronous bus controls, interrupt acknowledge, and bus arbitration are first-class timing events. NXP's M68000 manual provides separate byte/word read/write, read-modify-write, interrupt-acknowledge, and bus-arbitration timing descriptions.

## SLeeLa composition

SL68000CPU composes SLClock, SLRegisterFile, SLPipeline as an instruction/bus sequencing model, SLInstructionFetch, SLExecutionUnit, SLIOBus, SLIOTiming, SLInterruptController, SLBusArbiter, and SLCoupler.

The original CPU does not receive modern L1/L2/L3 cache, MMU, TLB, or integrated DMA objects.

## Variant rule

68008, 68010, 68020, 68030, 68040, 68060, CPU32, ColdFire, and integrated 68K processors remain separate profiles when their bus width, pipeline, cache, MMU, DMA, or exception architecture differs.
