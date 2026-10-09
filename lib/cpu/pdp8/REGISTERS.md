# PDP-8 Registers and State

## Base state

- **AC:** 12-bit accumulator.
- **Link:** one-bit arithmetic/link state.
- **PC:** 12-bit program counter in the base profile.
- **Instruction register:** 12-bit fetched instruction, useful for tracing.
- **Current instruction address:** retained separately for page-relative addressing and diagnostics.

## Optional state

EAE and memory-extension registers or fields are included only when the selected machine profile supports them. Do not invent a universal register file for all variants.

## Width rules

Mask accumulator, instruction, memory words, and base addresses to their documented widths. Keep the link bit separate from AC unless a specific instruction explicitly combines them for an operation.

## Debugger contract

Expose register name, width, current value, and optional-feature availability. Show unavailable state as absent rather than as a working register.
