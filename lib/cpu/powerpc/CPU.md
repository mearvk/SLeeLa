# PowerPC CPU

SLeeLa's PowerPC profile models the classic 32-bit PowerPC architecture while keeping architectural facilities separate from implementation-specific processor features.

## Architectural layers

PowerPC is defined in three useful layers:

- UISA — user instruction set architecture
- VEA — virtual environment architecture
- OEA — operating environment architecture

This separation is important because implementations can conform to different subsets and can vary substantially in caches, MMU, pipeline, and system integration. citeturn0search20turn0search1

## Core programmer model

- 32 general-purpose registers, 32-bit each
- 32 floating-point registers, 64-bit each
- 32-bit Condition Register
- 32-bit Link Register
- 32-bit Count Register
- 32-bit XER
- 32-bit instruction words
- 32-bit effective addresses for the base 32-bit profile
- MSR and other privileged state at the OEA level

IBM documents the CR, LR, CTR, GPR, XER, FPR, and FPSCR as the principal user-visible PowerPC register resources. citeturn0search1

## SLeeLa composition

SLPowerPCCPU composes SLClock, SLRegisterFile, SLPipeline, SLInstructionFetch, SLExecutionUnit, floating-point execution where implemented, SLMMU/SLTLB at OEA/implementation level, cache objects at implementation level, SLIOBus, SLInterruptController, SLBusArbiter, and SLCoupler.

## Variant rule

PowerPC 601, 603, 604, 750/G3, 7400/G4, 970/G5, embedded 4xx/8xx, PowerQUICC, and PowerPC 64-bit implementations remain separate profiles where implementation details materially differ.
