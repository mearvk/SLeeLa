# Cortex-M Architecture

## Execution model

Cortex-M processors use the Thumb instruction set family. Instruction availability and encoding rules vary across ARMv6-M, ARMv7-M, ARMv7E-M and ARMv8-M profiles. Thumb-2 is not universally available across the family. A-profile AArch32/AArch64 assumptions do not apply.

## Core pipeline

Represent fetch, decode, execute, and architectural commit as logical stages. Concrete pipeline depth, speculation, and latency are implementation-specific and should not be inferred from the profile alone. Exceptions and memory stalls may alter observable timing.

## Exceptions and interrupts

The core integrates with an NVIC-style interrupt controller. Model exception priorities, pending and active state, vector lookup, stacking/unstacking, exception return, and configurable fault escalation as supported by the selected profile. Tail-chaining, late arrival, and other latency optimizations should be enabled only where applicable and documented for the target.

## Privilege and security

Thread and Handler modes and privilege rules are profile-defined. ARMv8-M security extensions may provide Secure and Non-secure states, attribution controls, and banked state. TrustZone-M must be explicitly enabled; it is not a universal Cortex-M feature.

## Memory and protection

Accesses are routed through the selected memory map and bus model. An MPU may be present; a general-purpose MMU is not assumed. Alignment, access permissions, execute-never rules, and fault reporting follow the profile and target configuration.

## DSP and vector extensions

DSP multiply/accumulate and saturating operations are available only on profiles/variants that implement them. MVE/Helium is a distinct optional extension for applicable cores. Decode tables must gate every extension by configuration.

## Separation of concerns

NVIC, SysTick, MPU, FPU, debug/trace, cache controllers, DMA, and vendor peripherals should be explicit components or attached devices. Do not model DMA as a mandatory integrated CPU unit.
