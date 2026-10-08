# Intel Itanium CPU

SLeeLa models IA-64/Itanium as an explicitly parallel architecture rather than treating it as x86-64.

## Architectural model

Itanium uses Explicitly Parallel Instruction Computing (EPIC), with the compiler and instruction stream exposing instruction-level parallelism through bundles, templates, and stop information.

SLItaniumCPU composes:

- instruction fetch;
- bundle/template decoder;
- predicate register file;
- general register file;
- floating-point register file;
- branch register file;
- execution clusters;
- load/store;
- register rotation;
- speculation/checking;
- register stack engine;
- system/privileged state;
- MMU/TLB;
- cache hierarchy;
- interrupt/exception control;
- system interconnect.

IA-64 and x86 are separate instruction-set architectures. x86 compatibility was supplied by platform/processor mechanisms, not by making IA-64 instructions x86 instructions.
