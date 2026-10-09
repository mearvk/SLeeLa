# PDP-14 Timing and Scan Semantics

No cycle-accurate timing is claimed by the baseline profile.

- **Functional mode:** deterministic execution of supported control instructions.
- **Scan mode:** uses explicit input-sampling and output-commit boundaries.
- **Timed mode:** requires verified timing metadata from a hardware-specific profile.
- **Unknown timing:** reported as unavailable, never inferred from a different PDP family.

Control-system correctness depends on ordering and sampling boundaries, not host wall-clock speed. A simulator must identify whether outputs commit per instruction or at a scan boundary.