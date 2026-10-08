# PowerPC Specification

## Programmer-visible registers

| Resource | Width | Count |
|---|---:|---:|
| GPR | 32-bit | 32 |
| FPR | 64-bit | 32 |
| CR | 32-bit | 1 |
| LR | 32-bit | 1 |
| CTR | 32-bit | 1 |
| XER | 32-bit | 1 |
| FPSCR | 32-bit | 1 |

IBM documents these resources for 32-bit PowerPC applications. citeturn0search1

## Instruction format

PowerPC instructions are fixed 32-bit words. The consistent fixed-length encoding supports efficient decoding and pipelining. citeturn0search9

## Execution classes

SLeeLa models:

- integer/fixed-point operations;
- logical and shift operations;
- load/store;
- branch and condition processing;
- floating-point operations when FPU is present;
- system-control operations;
- exception/interrupt operations.

## Branching

PowerPC uses explicit branch, condition-register, link-register, and count-register mechanisms. Conditional branches can optionally update the Link Register when LK is set. citeturn0search11

## Memory management

The base architecture does not force one concrete MMU/TLB organization. The OEA defines the operating-environment memory-management model, while particular processors may implement different translation structures. citeturn0search20

## Cache

Cache behavior belongs primarily to the VEA and implementation layers. Cache sizes, associativity, levels, and coherency structures are therefore not invented in the generic CPU profile. citeturn0search20

## DMA

DMA is a system/implementation facility. External DMA masters connect through SLBusArbiter rather than being inserted into the generic PowerPC execution core.
