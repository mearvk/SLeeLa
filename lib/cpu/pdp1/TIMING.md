# PDP-1 Timing

The baseline SLeeLa PDP-1 profile does not claim cycle accuracy.

- **Functional mode:** deterministic instruction semantics.
- **Timed mode:** use explicit per-instruction and per-device timing metadata from a verified profile.
- **Missing data:** report timing as unavailable rather than inventing cycles or borrowing timings from PDP-4, PDP-7, PDP-10, or later DEC machines.

Host execution time is not guest instruction time. Identify timing values as documented, measured, estimated, or unspecified.