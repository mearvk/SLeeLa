# SPARC Specification

## SPARC V8

- 32-bit integer registers;
- register-window organization;
- 32-bit instruction words;
- delayed-control-transfer architecture;
- integer, load/store, branch, trap, and floating-point facilities as implemented.

## SPARC V9

- 64-bit integer registers;
- 64-bit PC/nPC/state registers;
- 64-bit virtual-address capability;
- 32-bit instruction words;
- expanded trap/state architecture;
- additional floating-point state.

Oracle documentation identifies V9 changes including widening integer registers, PC/nPC and FSR to 64 bits, and adding state such as CWP, PIL, TBA, PSTATE, TL, TPC, TNPC, TSTATE, and window-management registers. citeturn0search0turn0search1

## Register windows

Each window provides:

- 8 globals;
- 8 ins;
- 8 locals;
- 8 outs.

The outs of one window overlap the ins of the next. Implementations can provide different numbers of windows. citeturn0search9

## Floating point

SPARC V9 supports 32-, 64-, and 128-bit floating-point data types, with implementation-specific FPU behavior. citeturn0search20
