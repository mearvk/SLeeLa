# MIPS Specification

## Programmer-visible state

| Resource | Width | Count |
|---|---:|---:|
| General-purpose registers | 32-bit | 32 |
| PC | 32-bit | 1 |
| HI | 32-bit | 1 |
| LO | 32-bit | 1 |
| CP0 registers | implementation-defined control state | implementation-dependent |

GPR 0 is architecturally constant zero. Writes to it do not produce a persistent nonzero value.

## Instruction formats

Classic MIPS uses fixed 32-bit instructions with R, I, and J encoding families.

SLeeLa keeps instruction decode separate from execution so fields can feed register selection, immediate generation, ALU control, memory control, and branch control.

## Execution

The model includes:

- register-register ALU operations;
- immediate operations;
- loads and stores;
- conditional branches;
- jumps and jump-and-link;
- shifts;
- multiply/divide with HI/LO;
- system-control operations through CP0;
- exception and interrupt entry.

## Pipeline

The base model represents the classic five conceptual stages:

1. IF
2. ID
3. EX
4. MEM
5. WB

This is a functional timing abstraction, not a claim that every MIPS implementation physically used exactly five stages.

## Branch delay

Classic MIPS executes the instruction in the branch delay slot. citeturn0search20

## Privilege and CP0

CP0 contains privileged system state including status and exception-related state. MIPS documentation shows the CP0 Status register controlling interrupt, exception, privilege, and related operating state. citeturn0search19

## Cache/MMU/DMA boundary

Cache and TLB organization are implementation-dependent. The base ISA description does not invent a particular cache size or TLB entry count. DMA is modeled as an external bus master unless a specific MIPS SoC/core documents otherwise.
