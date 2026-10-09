# PDP-8 Specification and Profile Contract

## Configuration

- `machine_model`: named PDP-8 variant or explicit abstract profile.
- `memory_words`: configured word count, bounded by the selected model and installed options.
- `extended_arithmetic`: enables EAE behavior only where supported.
- `memory_extension`: enables fields and instructions only where supported.
- `io_devices`: explicit device registry and IOT handlers.
- `strict_profile`: reject unsupported operations by default.

## Word and address model

The base architecture uses 12-bit words and a 12-bit program counter/address. Mask values at architectural boundaries. Larger memory configurations require the appropriate documented memory-extension model rather than silently widening addresses.

## Execution contract

1. Fetch one 12-bit instruction from configured memory.
2. Decode the instruction group and fields according to the selected profile.
3. Resolve page and indirect addressing with documented auto-index behavior.
4. Apply arithmetic and link-bit semantics at 12-bit width.
5. Route IOT operations to configured device handlers.
6. Report illegal or unsupported operations through structured diagnostics.

## Optional facilities

Extended arithmetic, memory extension, and device behavior are not universal across every PDP-8 configuration. Keep option-specific state and instructions disabled unless selected.

## Safety

Validate every memory reference against installed memory and active fields. Never treat a guest address as an unchecked host-memory index.
