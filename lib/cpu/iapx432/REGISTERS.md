# Intel iAPX 432 Registers

The iAPX 432 does not use the conventional programmer-facing general-purpose register architecture of x86, MIPS, or RISC-V.

SLeeLa therefore models architectural state through:

- instruction/control state;
- object references;
- descriptor state;
- execution context;
- protection state;
- memory-management state;
- processor status.

Implementation-visible temporary state is not exposed as a universal programmer register file.
