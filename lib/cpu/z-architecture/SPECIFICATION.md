# z/Architecture Specification and Profile Contract

## Configuration

Required fields:
- `architecture_level`: named documented architecture generation.
- `enabled_facilities`: explicit facility set for the selected machine.
- `addressing_mode` and `address_translation`: configured execution and system model.
- `storage_map`: main storage, reserved ranges, and platform-defined regions.
- `privilege_model`: problem/supervisor state and relevant control state.
- `interrupt_model`: configured program, external, I/O and machine-check events.
- `strict_facilities`: defaults to true.

## Architectural state

Model the 16 64-bit general-purpose registers and condition code, plus the program status word and architecture-level control state required by the selected profile. Floating-point registers, vector registers, access registers, control registers, prefix/lowcore-related state, and other special facilities must be included only with their documented architecture semantics.

The exact PSW format, address interpretation, and special-register inventory depend on the selected architecture level and execution environment.

## Execution requirements

1. Decode instructions only if available at the configured architecture level and facility set.
2. Preserve condition-code behavior and specified arithmetic exceptions.
3. Validate instruction alignment, operand addressing, access permissions and translation outcomes.
4. Model program interruptions and architected exception state.
5. Distinguish privileged operations from problem-state operations.
6. Keep operating-system and machine-model behavior separate from CPU instruction semantics.

## Storage and translation

Translation, protection, address-space identifiers, page tables, storage keys and related controls require an explicit system configuration. Do not treat main storage as an unprotected flat host array.

## Facilities

Vector, floating-point, decimal, cryptographic, transactional, compression, and other optional facilities are independently gated by architecture level and machine configuration. Facility availability must not be inferred from a generic “z/Architecture” label.

## Errors

Invalid configurations fail before execution. Unsupported instructions or unavailable facilities raise the configured architected program interruption or a structured diagnostic. Unknown behavior remains explicitly unspecified.
