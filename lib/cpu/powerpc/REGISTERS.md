# PowerPC Registers

## General-purpose

GPR0-GPR31 are 32-bit registers in the 32-bit profile.

## Floating-point

FPR0-FPR31 are 64-bit floating-point registers.

## Branch state

- CR — 32-bit Condition Register, divided into eight 4-bit fields
- LR — 32-bit Link Register
- CTR — 32-bit Count Register

The CR's eight independently usable four-bit fields are documented by IBM. citeturn0search12

## Arithmetic status

XER records fixed-point exception state including carry/overflow-related information.

## Floating-point status

FPSCR records floating-point status and control.

## Privileged state

MSR and implementation/OEA-specific SPRs are modeled separately from the user register file.

Special-purpose registers are accessed through the PowerPC SPR mechanisms; IBM documentation describes SPR control of interrupts, memory management, caches, timers, and other processor resources. citeturn0search12
