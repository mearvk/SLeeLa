# SPARC Timing

The generic timing model separates architectural sequencing from processor-specific implementation timing.

Tracked events include:

- fetch;
- decode;
- register-window access;
- integer execution;
- branch/delayed transfer;
- load/store;
- FPU operation;
- cache access;
- TLB/translation;
- window spill/fill;
- trap;
- interrupt;
- retirement.

SPARC V9 uses 32-bit instruction words, while actual execution pipelines vary by implementation. citeturn0search20turn0search22

The model does not assign UltraSPARC timing to generic SPARC.
