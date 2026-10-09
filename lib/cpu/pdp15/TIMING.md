# PDP-15 Timing

Functional mode is the default and does not claim cycle accuracy.

- **Functional mode:** deterministic execution of implemented semantics.
- **Timed mode:** uses explicit timing metadata for the selected processor model, instruction, and relevant option/peripheral.
- **Missing data:** report timing as unavailable; do not copy PDP-4, PDP-7, or PDP-9 cycle counts.
- **Provenance:** identify timing values as documented, measured, or estimated.

Host runtime is not guest CPU timing. Any timing approximation must be visible in reports and configuration.