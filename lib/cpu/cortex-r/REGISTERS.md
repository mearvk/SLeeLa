# Cortex-R Registers

## Profile-driven register state

The precise register bank is architecture-generation and execution-state dependent. The model should include only registers supported by the selected core.

Common categories include:
- General-purpose registers and program counter.
- Link register and architecture-defined status/condition registers.
- Banked registers and saved status state for supported exception modes.
- System-control, protection, memory-attribute and translation registers where implemented.
- Optional FPU, virtualization, debug and trace registers.

## Rules

1. Use target-documented reset values and register widths.
2. Model special-register access restrictions and side effects.
3. Preserve banked state during exception entry and return.
4. Reject access to registers absent from the selected profile.
5. Preserve reserved-bit semantics as specified; never assign invented meanings.
6. Mark implementation-defined registers as `unspecified` until a concrete core configuration supplies their definitions.

## Introspection

Expose a profile-aware register inventory to the SLeeLa debugger. Reports should identify each register's width, access mode, reset source, bank or execution mode, and evidence classification.
