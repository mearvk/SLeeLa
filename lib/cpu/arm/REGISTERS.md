# ARM Registers

## AArch64

- X0-X30: 64-bit general-purpose registers
- W0-W30: low 32-bit views of X registers
- SP: stack pointer
- PC: program counter
- PSTATE: processor state
- V0-V31: 128-bit SIMD/FP registers

Arm's A64 register documentation describes X/W register views and the 31-register general-purpose set. citeturn0search21

## AArch32

- R0-R15
- CPSR
- SPSR registers as provided by the selected exception modes
- banked registers where the concrete profile requires them

## System registers

AArch64 system registers control exception state, memory translation, timers, debug, cache maintenance, and other privileged functions. They are represented through a typed system-register interface rather than being mixed into the GPR file.

## FP/SIMD

V0-V31 provide the AArch64 SIMD/FP register file. Concrete implementation profiles determine supported FP/SIMD instruction subsets.
