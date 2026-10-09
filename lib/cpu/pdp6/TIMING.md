# PDP-6 Timing

This profile does not claim cycle-accurate PDP-6 timing by default.

- **Functional mode:** deterministic instruction semantics without timing claims.
- **Timed mode:** use explicit per-operation timing metadata from a selected PDP-6 machine profile.
- **Unknown timing:** mark unavailable; do not substitute PDP-10, KI, KL, or later-family timing data.

A timing profile should identify its source and distinguish documented, measured, and estimated values. Host execution speed is not guest instruction timing.