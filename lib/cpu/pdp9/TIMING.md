# PDP-9 Timing

Cycle-accurate timing is not claimed by the baseline implementation.

- **Functional mode:** deterministic instruction semantics without cycle-count guarantees.
- **Timed mode:** uses verified per-operation and, where relevant, per-device timing metadata.
- **Missing data:** report timing as unavailable rather than borrowing PDP-4/PDP-7 or other-family timing.

Host execution duration is not guest instruction time. Timing metadata should identify its source and mark documented versus estimated values.