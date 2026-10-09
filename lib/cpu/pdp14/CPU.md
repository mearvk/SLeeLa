# DEC PDP-14 Industrial Processor

SLeeLa profile for the DEC PDP-14 industrial control processor, designed for deterministic control tasks rather than general-purpose computing.

- **Status:** profile-driven functional model; not a cycle-accurate PLC emulator.
- **Role:** industrial sequencing and logic control.
- **Execution model:** control-oriented instruction operations over configured input/output points and internal state.
- **Configuration:** I/O point map, memory/state layout, instruction semantics, and timing are supplied by the selected PDP-14 profile.
- **Compatibility:** do not treat PDP-14 as a PDP-11/PDP-8 general-purpose instruction-set variant.

Unsupported or unverified instruction behavior must be reported, not guessed.