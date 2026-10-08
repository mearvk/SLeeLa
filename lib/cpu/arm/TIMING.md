# ARM Timing

## Architectural versus implementation timing

ARM instruction timing varies strongly across ARM generations and implementations.

SLeeLa records:

- instruction fetch;
- decode;
- issue;
- execution unit;
- dependency/stall;
- branch prediction/result;
- load/store;
- cache hit/miss;
- TLB/translation walk;
- exception;
- retirement.

A Cortex-M profile can use a relatively direct pipeline model, while a Cortex-A or Neoverse profile can supply wider out-of-order and speculative timing.

## AArch64 instruction size

A64 instructions are fixed 32-bit words. citeturn0search20

## AArch32/Thumb

A32 and T32 have different instruction encodings and timing characteristics. The decoder and timing model therefore track instruction-state explicitly.
