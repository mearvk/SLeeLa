# SLeeLa ARM Cortex-M CPU Family

**Status:** Family-level behavioral model  
**Path:** `/lib/cpu/cortex-m/`  
**Evidence labels:** documented = architecture-defined; profile-dependent = requires selected core profile; unspecified = not inferred.

## Purpose

Models ARM Cortex-M microcontroller processor profiles for SLeeLa's CPU and SLVM tooling. Cortex-M is a family, not one interchangeable microarchitecture. A configured core profile controls available instructions, registers, exceptions, memory protection, security, FPU, caches, and timing.

## Supported profile identifiers

- `ARMv6-M`: Cortex-M0/M0+ class baseline.
- `ARMv7-M`: Cortex-M3 class.
- `ARMv7E-M`: Cortex-M4/M7 class; DSP extensions and FPU are variant-dependent.
- `ARMv8-M Baseline`: Cortex-M23 class.
- `ARMv8-M Mainline`: Cortex-M33 class; security extension is configuration-dependent.
- `Helium/MVE`: Cortex-M55/M85-class vector extension when explicitly enabled.

These labels describe architectural profiles, not identical cycle timing or peripheral sets. Implementations may add vendor-specific features.

## Model responsibilities

The SLeeLa model covers instruction decode and execution, register and special-register state, exception entry/return, NVIC-facing interrupt behavior, memory accesses, optional MPU/FPU/security features, and profile-aware faults. SoC peripherals and DMA engines are modeled separately and attached through the bus.

## Non-goals

This is not an A-profile or AArch64 model. It does not assume a full operating system, MMU, data cache, FPU, TrustZone-M, or identical timing across Cortex-M variants. See `SPECIFICATION.md` and `ARCHITECTURE.md`.

## Conformance

A profile must reject unsupported instructions and unavailable registers predictably. Unknown behavior is reported as unspecified rather than silently emulated. Timing is exact only where a selected implementation and memory/peripheral model provide enough information.
