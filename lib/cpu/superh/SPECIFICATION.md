# SuperH Specification

## Common architecture

- 32-bit RISC family
- 16-bit fixed-length instructions
- load/store architecture
- delayed branches
- compact C-oriented instruction set
- 16 primary 32-bit general registers
- control/system registers
- MACH/MACL multiply-accumulate state
- procedure register
- program counter

Renesas documents the SH-1/SH-2 register organization and SH-4's 16 general registers plus shadow registers and control/system state. citeturn1search21turn1search20

## SH-4 profile

SH-4 adds superscalar execution, FPU support, MMU/TLB resources, separate instruction/data caches, and a five-stage pipeline. Renesas documents simultaneous execution of up to two instructions per cycle. citeturn1search20turn1search23

## Compatibility

Compatibility is modeled at the instruction/object-code level where documented, not as identical microarchitecture.
