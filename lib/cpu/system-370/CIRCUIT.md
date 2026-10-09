# System/370 Logical Circuit Abstraction

## Scope

This describes logical simulation blocks, not a transistor-level IBM System/370 circuit design.

## Functional blocks

- Instruction fetch, decode, and execution control.
- General-purpose register file and condition-code logic.
- System/370 PSW and interruption-state control.
- Integer arithmetic, branch, and storage-operation units.
- Optional floating-point and control-state blocks.
- Optional DAT and protection-checking path.
- Storage interface and channel-I/O request interface.

## Configuration

Instantiate only facilities supported by the selected architecture profile. Keep machine-specific storage, channels, devices, timers, and cache structures outside the generic core unless evidence supports a particular integration.

## Limits

Do not infer gate counts, physical layout, clock frequency, power draw, or undocumented pipeline depth from this abstraction.
