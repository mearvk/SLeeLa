# System/370 Registers

## General-purpose registers

System/370 defines sixteen 32-bit general-purpose registers, R0 through R15. Preserve 32-bit arithmetic and addressing semantics even when the host implementation uses wider types.

## Floating-point and control state

Floating-point registers and control registers must be represented only when supported by the selected architecture profile. Register count, access rules, and instruction availability must follow the target documentation.

## PSW and condition code

The PSW and condition code are architectural state. Implement the System/370 format and transitions for the selected profile; do not substitute System/360 or z/Architecture layouts without an explicit, validated compatibility mapping.

## Debugger requirements

Expose named registers, widths, current values, privilege-sensitive state, and evidence classifications. Avoid exposing reserved or unavailable state as usable architectural registers.
