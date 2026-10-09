# System/360 Specification and Profile Contract

## Required configuration

- `machine_model`: documented System/360 model or explicitly named abstract profile.
- `instruction_profile`: valid instruction subset and model-dependent options.
- `storage_size` and `storage_map`: configured main storage and device regions.
- `protection_model`: supported storage protection behavior, if present.
- `interrupt_model`: program, external, supervisor-call and I/O interruption behavior.
- `channel_model`: configured channel subsystem and devices.
- `strict_profile`: defaults to true.

## Architectural state

Model 16 32-bit general-purpose registers, four 64-bit floating-point registers where the selected model supports the floating-point feature, the condition code, and the architecture-defined PSW and interruption state. Register and feature availability must follow the selected machine model and documented architecture level.

The PSW includes control and execution state whose exact interpretation must be implemented from the chosen System/360 profile. Do not substitute a later architecture's PSW format.

## Execution requirements

1. Decode only instructions valid for the configured System/360 profile.
2. Preserve condition-code results and architected exception behavior.
3. Validate storage address, access width, alignment where required, and permissions.
4. Model program interruptions and PSW transitions according to the profile.
5. Enforce privileged operation restrictions.
6. Keep channel execution and device behavior in the machine/system model.

## Storage and I/O

Storage protection and channel capabilities varied by model and configuration. Enable them only when the selected profile documents support. Channel programs, device state, and I/O completion are not ordinary CPU registers.

## Compatibility

Do not enable System/370, ESA/390, or z/Architecture instructions as part of the base System/360 profile. Any compatibility extension must be explicitly named and validated.

## Error handling

Unsupported opcodes and unavailable features produce modeled operation exceptions or structured diagnostics. Invalid machine profiles fail before execution.
