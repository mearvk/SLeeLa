# ESA/390 Storage Hierarchy and Translation Performance

## Scope

Cache geometry and storage hierarchy are implementation-specific rather than universal ESA/390 architectural properties.

## Modeling levels

- **Architectural:** storage contents, translation, protection, and interruption results.
- **Model timing:** documented storage and translation latency for a named machine.
- **Microarchitecture:** caches, buffers, and translation lookaside structures only when supported by reliable model documentation.

## Correctness

Performance abstractions must not alter visible storage or translation semantics. Any translation cache must preserve invalidation, protection, and address-space behavior.

## Evidence

Identify the target model and label each latency or cache claim `documented`, `derived`, `estimated`, or `unspecified`.
