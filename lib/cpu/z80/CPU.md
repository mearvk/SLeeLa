# Zilog Z80 CPU

SLeeLa's Z80 profile models the classic Zilog Z80 as an 8-bit processor with a 16-bit address space and cycle-visible memory, I/O, and interrupt behavior.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Family | Zilog Z80 | documented |
| Data width | 8-bit | documented |
| Address width | 16-bit | documented |
| Address space | 64 KiB | derived |
| Main programmer register set | AF, BC, DE, HL, IX, IY, SP, PC | documented |
| Alternate register set | AF', BC', DE', HL' | documented |
| Interrupt registers | I, R | documented |
| Cache | None | architecture/documentation |
| Integrated DMA | None in base Z80 CPU | documented; Z80 DMA is a separate peripheral |
| MMU/TLB | None in base CPU | architecture |
| Modern superscalar pipeline | No | architecture |

## SLeeLa composition

SLZ80CPU composes SLClock, SLRegisterFile, SLPipeline as a machine-cycle sequencer, SLInstructionFetch, SLExecutionUnit, SLIOBus, SLIOTiming, SLInterruptController, and SLCoupler.

The base profile does not instantiate L2/L3 cache, MMU, TLB, or integrated DMA.

## Timing principle

Zilog describes execution in M (machine) cycles and T (clock) cycles. Memory read/write, I/O read/write, and interrupt acknowledge are distinct basic operations; WAIT can extend external-device synchronization. [Zilog Z80 CPU User Manual](https://www.zilog.com/docs/z80/z80cpu_um.pdf)

## Variant rule

Z80A, Z80B, CMOS Z84C00, Z180, eZ80 and system-specific derivatives should remain distinct profiles when their timing, bus, memory, or internal architecture differs.
