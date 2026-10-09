# z/Architecture I/O and DMA System Integration

## Scope

DMA and I/O data movement are system/channel-subsystem concerns, not assumed standalone CPU-core features. This document defines how the CPU model connects to a configurable mainframe I/O system.

## Configuration

Specify channel-subsystem behavior, device model, storage access, command/status formats, interruption routing, protection checks and concurrency rules from the target system contract.

## Storage protection

I/O data movement must honor system-defined address translation, storage protection and device access constraints. Invalid addresses and malformed command structures return modeled errors rather than accessing arbitrary host memory.

## CPU interaction

Model I/O instruction semantics and interruption delivery at the architectural level. Device execution, channel scheduling and transfer timing belong to the system model and should be separately reported.

## Unknowns

Do not invent channel features, transfer rates, device semantics or coherence guarantees. Mark unavailable target behavior as `unspecified`.
