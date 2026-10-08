# MIPS CPU

SLeeLa's MIPS profile models a classic 32-bit MIPS architecture, centered on the MIPS I-style programmer model rather than a particular later implementation.

## Identity

| Field | Value | Evidence |
|---|---|---|
| Architecture | MIPS 32-bit RISC | documented |
| General registers | 32 × 32-bit | documented |
| Register zero | GPR 0 reads as zero | documented |
| Instruction width | 32-bit | documented |
| Data/address model | 32-bit | documented |
| Pipeline model | classic staged RISC pipeline | implementation-dependent |
| Branch delay slot | yes for classic MIPS I-style model | documented |
| CP0 | system-control coprocessor | documented |
| Cache | implementation-dependent; not invented for ISA | architecture |
| DMA | external/system facility | architecture |

The SLeeLa implementation deliberately targets a classic MIPS profile. Modern MIPS cores vary substantially in cache hierarchy, pipeline depth, TLBs, multiprocessing, and instruction extensions; those should be separate implementations rather than silently folded into the base model.

## Composition

SLMIPSCPU composes SLClock, SLRegisterFile, SLPipeline, SLInstructionFetch, SLBranchPredictor where an implementation provides one, SLExecutionUnit, SLMMU/TLB only when the selected MIPS implementation documents them, SLIOBus, SLInterruptController, SLBusArbiter, and SLCoupler.

The classic architecture includes a branch delay slot: MIPS training material documents that the instruction following a branch or jump is executed while the new PC is installed. citeturn0search20

## Variant rule

MIPS I, MIPS II/III/IV, MIPS32, MIPS64, R-series implementations, embedded MIPS, and later MIPS cores must not be treated as identical CPUs. Cache, TLB, CP0, FPU, DSP, pipeline, and bus properties belong to the appropriate implementation.
