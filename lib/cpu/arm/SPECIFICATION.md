# ARM Specification

## AArch64 programmer model

| Resource | Width | Count |
|---|---:|---:|
| General registers X0-X30 | 64-bit | 31 |
| SP | 64-bit | 1 |
| PC | architectural state | 1 |
| PSTATE | architectural status | 1 |
| V0-V31 | 128-bit SIMD/FP | 32 |

Writes to a W register address the low 32 bits of the corresponding X register and architecturally zero-extend into X.

Arm documents the 31 general-purpose registers and their W/X views in A64. citeturn0search21

## AArch32 programmer model

AArch32 exposes R0-R15, CPSR, and SPSR state according to the selected exception/processor mode. ARM/Thumb instruction sets use 32-bit and 16/32-bit instruction encodings respectively.

## Instruction sets

The generic ARM model includes:

- A32;
- T32/Thumb;
- A64.

A concrete CPU profile selects its supported execution states and extensions.

## Execution classes

- integer arithmetic and logic;
- shifts/bit manipulation;
- loads/stores;
- branches;
- multiply/divide;
- system instructions;
- FP/SIMD/NEON where enabled;
- atomics where supported;
- exceptions and interrupts.

## Memory management

AArch64 virtual memory uses translation tables and a translation regime selected by system registers. The SLeeLa MMU/TLB model therefore belongs to the selected ARM implementation and operating regime rather than being hard-coded as one universal cache/TLB size.

## Security/privilege

Exception levels EL0-EL3 are modeled for AArch64 where implemented. Secure/Non-secure state and TrustZone-related behavior belong to the appropriate architecture profile.
