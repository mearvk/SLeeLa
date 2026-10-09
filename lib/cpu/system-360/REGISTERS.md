# System/360 Registers

## General-purpose registers

The architecture defines 16 32-bit general-purpose registers, conventionally R0-R15. Software may use registers for address calculations and linkage conventions, but the CPU model must not hardwire a particular ABI into their hardware semantics.

## Floating-point registers

Where the selected model supports the floating-point feature, model the architecture-defined floating-point register state and instruction behavior. Do not expose floating-point operations as universally available across all System/360 configurations.

## Condition code and PSW

The condition code and program status word are architectural state. Implement their documented fields, state transitions, and interruption behavior for the selected profile; do not reuse later PSW formats without verification.

## Rules

1. Track register width, access semantics, and feature availability.
2. Preserve architected condition-code and exception effects.
3. Keep reserved or model-dependent state faithful to documentation.
4. Reject unavailable operations and inaccessible privileged state.
5. Expose register descriptions to the debugger with evidence classifications.
