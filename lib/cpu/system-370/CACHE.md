# System/370 Storage Hierarchy and Cache Model

## Scope

Cache and buffer organization is implementation-specific, not a universal architectural contract for all System/370 machines.

## Layers

- **Architectural storage:** data, address translation, protection, and interruption behavior.
- **Model timing:** documented storage access and wait-state characteristics.
- **Optional microarchitecture:** caches, buffers, and related behavior only when verified for the chosen machine.

## Translation

If DAT is enabled, translation lookaside behavior may be represented as an optional performance abstraction. It must not change architecturally visible translation, protection, or invalidation behavior.

## Evidence

Record the target model and evidence class for latency, cache geometry, and translation performance. Use `unspecified` where reliable data is unavailable.
