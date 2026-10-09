# System/370 Specification and Profile Contract

## Configuration

A machine profile must identify:
- System/370 model or explicitly abstract architecture profile.
- Base architecture level and enabled facilities.
- Main-storage size, storage map, and access rules.
- Address-translation mode and translation-table configuration when supported.
- Interruption and timer model.
- Channel and device configuration.
- Strict handling of unsupported operations.

## Architectural state

Model sixteen 32-bit general-purpose registers, condition-code state, and a System/370-appropriate PSW. Add floating-point registers and control registers only according to the selected architecture level and machine configuration. Do not reuse z/Architecture register or PSW layouts.

## Execution requirements

1. Decode only instructions defined for the selected profile.
2. Preserve instruction length, condition-code effects, storage semantics, and program interruptions.
3. Separate logical/virtual addresses from real storage addresses when DAT is enabled.
4. Apply access protection and translation checks before storage access.
5. Enforce privileged-operation rules and interruption-state transitions.
6. Route channel I/O through the configured system model.

## Facility handling

Facilities such as dynamic address translation, extended control functions, and floating-point operations must be explicitly enabled only when documented for the target. Later ESA/390 and z/Architecture features are excluded unless modeled as a separate, named extension.

## Failure behavior

Invalid profiles fail during configuration. Unsupported instructions or disabled facilities generate a modeled operation exception or a structured diagnostic; they must not silently execute as no-ops.
