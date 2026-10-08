# RISC-V Timing

RISC-V specifies architectural behavior while leaving most pipeline timing to the implementation.

SLeeLa records:

- instruction fetch;
- decode;
- register read;
- execute;
- memory access;
- extension-unit latency;
- dependency stalls;
- branch prediction/result;
- cache hit/miss;
- TLB/page-table walk;
- exception/interrupt;
- retirement.

## Instruction alignment

The base RV32I ISA uses 32-bit instructions and IALIGN=32. An instruction extension supporting 16-bit instructions can relax alignment to IALIGN=16. citeturn0search0

## Microarchitectural profiles

Single-cycle, multicycle, in-order pipeline, superscalar, speculative, and out-of-order implementations can be represented without changing the ISA model.
