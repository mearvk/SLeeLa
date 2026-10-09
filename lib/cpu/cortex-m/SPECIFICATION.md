# Cortex-M Specification and Profile Contract

## Configuration

Required configuration:
- `profile`: one of ARMv6-M, ARMv7-M, ARMv7E-M, ARMv8-M-Baseline, ARMv8-M-Mainline.
- `core_variant`: optional implementation label (for example M0+, M3, M4, M7, M23, M33, M55, M85).
- `extensions`: explicit feature set, including DSP, FPU, MVE/Helium, TrustZone-M, MPU, cache, debug and trace.
- `memory_map`, `clock_hz`, `memory_wait_states`, `interrupt_sources`: target/SoC configuration.
- `endianness`: target-defined supported mode; validate against the selected core and implementation.
- `strict_profile`: default true; reject unsupported encodings and unavailable features.

## Architectural state

Common state includes R0-R12, stack-pointer bank selection where implemented (MSP and optionally PSP), LR, PC, xPSR, and profile-defined special registers. FPU state (S0-S31 and FPSCR where applicable), security-state banked registers, MPU registers and debug registers exist only when configured and accessible.

## Required behavior

1. Decode only the instruction encodings permitted by the selected architecture profile and enabled extensions.
2. Preserve architectural condition flags and instruction-width semantics.
3. Model exception entry/return and stack alignment according to profile and configured implementation.
4. Route eligible interrupts through NVIC-compatible priority and pending/active state.
5. Raise appropriate fault behavior for invalid encodings, privilege violations, memory protection violations, and invalid exception returns where defined.
6. Keep core behavior separate from MCU peripherals and SoC-specific memory maps.

## Optional capabilities

MPU, FPU, DSP instructions, MVE, TrustZone-M, instruction/data cache, debug/trace, and vendor-defined features must be explicit profile options. Cache presence and size are not inferred solely from the Cortex-M family name.

## Timing contract

Instruction cycle counts are profile- and implementation-specific. Report `documented`, `derived`, `estimated`, or `unspecified` timing evidence. Do not treat a generic cycle estimate as a silicon guarantee.

## Errors

Configuration errors fail before execution with the field and unsupported feature identified. At runtime, unsupported opcodes and illegal state transitions produce a profile-appropriate fault or a structured emulator diagnostic; never silently execute as a different instruction.
