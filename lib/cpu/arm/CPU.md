# ARM CPU

SLeeLa's ARM profile models the ARM architectural family while keeping AArch32 and AArch64 distinct from individual Cortex and Neoverse microarchitectures.

## Architectural profiles

- AArch32: ARM 32-bit execution state, including ARM and Thumb instruction sets as applicable.
- AArch64: 64-bit execution state defined by the Armv8-A and later AArch64 architecture.

Arm documents AArch64 as a 64-bit execution state with 31 general-purpose 64-bit registers, a program counter, stack pointer, and PSTATE. citeturn0search20

## Programmer model

AArch64 provides X0-X30, SP, PC, PSTATE, and SIMD/FP V0-V31. AArch32 provides R0-R15 plus CPSR/SPSR state and optional VFP/NEON resources.

## SLeeLa composition

SLARMCPU composes SLClock, SLRegisterFile, SLInstructionFetch, SLPipeline, SLExecutionUnit, branch/control logic, SIMD/FP where enabled, SLMMU/SLTLB, cache hierarchy, SLInterruptController, SLIOBus, SLBusArbiter, and SLCoupler.

## Variant rule

ARM7, ARM9, ARM11, Cortex-A, Cortex-R, Cortex-M, Neoverse, and AArch64 cores must not be treated as one microarchitecture. Pipeline, cache, MMU, exception model, bus interface, and optional extensions vary materially.
