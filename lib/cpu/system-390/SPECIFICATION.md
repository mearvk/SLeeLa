# ESA/390 Specification and Profile Contract

## Configuration

Each machine profile must specify:
- System/390 model and ESA/390 architecture level.
- Supported addressing modes and enabled facilities.
- Installed storage, translation configuration, and protection behavior.
- Interrupt, timer, and channel configuration.
- Strict behavior for unsupported operations.

## State model

Represent sixteen 32-bit general-purpose registers and the architecture-defined control, access, floating-point, condition-code, and PSW state applicable to the selected profile. The 64-bit host type does not itself authorize 64-bit architectural operations.

## Execution contract

1. Decode only operations available in the configured ESA/390 profile.
2. Preserve architected instruction lengths, operand semantics, condition codes, and interruptions.
3. Route storage access through translation and protection checks.
4. Enforce privilege and addressing-mode constraints.
5. Model program, external, supervisor-call, and I/O interruption transitions.
6. Keep channel and device execution in the system model.

## Addressing and facilities

ESA/390 supports architecture-specific addressing and translation facilities; the exact behavior must be selected by the named profile. z/Architecture-only 64-bit addressing and instructions remain disabled unless a separate architecture profile explicitly implements them.

## Error behavior

Reject invalid configurations before execution. Unsupported opcodes or disabled facilities generate a modeled exception or structured diagnostic rather than silently succeeding.
