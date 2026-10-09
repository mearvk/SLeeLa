# Cortex-M Registers

## Core registers

- R0-R12: general-purpose registers.
- SP: active stack pointer; MSP is used in Handler mode. PSP is available where implemented and selected for Thread mode.
- LR: link register, including architecture-defined exception-return encodings.
- PC: program counter, with execution semantics defined by the selected Thumb profile.
- xPSR: combined program status view including condition flags, execution state, and exception number fields as architecturally defined.

## Special registers

CONTROL, PRIMASK, BASEPRI, FAULTMASK, IPSR, MSP, PSP, and other special registers are profile- and privilege-dependent. BASEPRI and FAULTMASK, for example, are not available identically on all baseline profiles. Implement access checks rather than exposing every register universally.

## Optional register banks

- FPU: floating-point registers and FPSCR only if the FPU is present and enabled.
- ARMv8-M security: Secure/Non-secure banked stack pointers and relevant special state where implemented.
- MPU: region and control registers according to the implementation.
- Debug/trace: implementation-defined registers and access restrictions.

## Modeling requirements

1. Keep register width and reset values in the selected profile/implementation definition.
2. Implement read/write side effects for special registers.
3. Enforce privilege and security attribution restrictions.
4. Preserve architecturally defined reserved-bit behavior; do not invent meanings for reserved bits.
5. Record unknown or vendor-specific register behavior as `unspecified` until configured.
