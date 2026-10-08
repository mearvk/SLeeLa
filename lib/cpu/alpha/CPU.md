# DEC Alpha CPU

SLeeLa models the DEC Alpha architecture as a 64-bit RISC family, keeping Alpha EV4 through EV8-era implementations distinct from the architectural ISA.

Alpha was designed as a clean 64-bit RISC architecture with fixed-length instructions and a large register set.

## Core composition

SLAlphaCPU composes:

- instruction fetch/decode;
- integer register file;
- floating-point register file;
- integer execution;
- floating-point execution;
- load/store;
- branch/control;
- PALcode/system-control interface;
- MMU/TLB;
- cache hierarchy;
- pipeline/issue/retirement;
- interrupts/exceptions;
- system interconnect.

## Architectural boundary

EV4, EV5, EV6, EV7, and EV8-era designs differ substantially in pipeline depth, issue width, prediction, caches, and execution resources. Those properties are implementation profiles rather than universal Alpha requirements.
