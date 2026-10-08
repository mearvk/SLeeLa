# SuperH Registers

## General registers

- R0-R15: 32-bit general-purpose registers

## System registers

- PC: program counter
- PR: procedure/return register
- MACH: multiply-accumulate high
- MACL: multiply-accumulate low

## Control registers

The family includes status/control registers such as SR and GBR/VBR; later profiles add MMU, exception, cache, and system-control registers.

SH-4 adds shadow registers and additional control/system state. citeturn1search20turn1search21

## Profile-specific state

FPU, DSP, MMU, cache, debug, and exception registers are attached only to profiles that document them.
