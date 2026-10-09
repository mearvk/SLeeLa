# z/Architecture Storage Hierarchy and Cache Notes

## Scope

The CPU model must not invent a universal cache size, associativity, replacement policy, or latency for z/Architecture processors. Cache and storage-hierarchy behavior varies by processor generation and implementation.

## Modeling levels

- **Architectural level:** model memory ordering and storage semantics required by the architecture.
- **Performance abstraction:** optional latency model parameterized by documented processor and memory information.
- **Microarchitectural detail:** only implement cache geometry, sharing, prefetching, and maintenance behavior when reliable target-specific data is available.

## Main storage

Represent main storage and address translation separately from any cache-performance abstraction. Storage protection, translation, and access exceptions must remain correct even when cache timing is not modeled.

## Evidence and reporting

Every timing or cache parameter carries an evidence label. Unknown values remain `unspecified`. Do not expose guessed cache geometry as an architectural guarantee.
