# ARM Architecture

## AArch64 datapath

SLARMCPU
-> instruction fetch
-> decoder
-> register file
-> integer/branch units
-> load/store unit
-> FP/SIMD unit
-> MMU/TLB
-> cache/interconnect
-> exception control

AArch64 instructions are fixed 32-bit words, giving a different decode model from x86's variable-length instructions. citeturn0search20

## AArch32

The architecture layer selects ARM or Thumb instruction state and exposes the corresponding PC/CPSR/exception behavior.

## Pipeline

SLPipeline represents fetch, decode, issue, execute, memory, and retirement as functional stages. Actual Cortex pipeline depth, width, forwarding, speculation, and out-of-order behavior belong to the concrete processor implementation.

## Branching

Branch and compare operations feed explicit control-flow state. Conditional execution differs between A32 and T32 and should not be generalized from one ARM generation to another.

## Load/store

Address generation and memory access are first-class operations. The load/store unit connects through MMU/TLB and cache/interconnect layers.

## Exceptions

AArch64 exception levels and vector entry are represented through a dedicated exception controller. AArch32 CPSR/SPSR and processor modes remain separate state.
