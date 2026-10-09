# System/360 Storage Hierarchy and Cache Model

## Scope

Cache behavior is implementation-dependent and must not be assumed to be a universal System/360 architectural feature. Some models used different storage technologies and performance characteristics.

## Modeling levels

- **Architectural:** storage results, protection, interruption, and ordering behavior.
- **Performance abstraction:** configurable access latency based on documented machine data.
- **Implementation detail:** cache or buffer behavior only when supported by the selected model's documentation.

## Main storage

Represent installed main storage, address ranges, access restrictions, and wait states in the machine profile. Keep these separate from optional performance abstractions.

## Evidence

Label storage latency as `documented`, `derived`, `estimated`, or `unspecified`. Do not assign modern cache structures to historical models without evidence.
