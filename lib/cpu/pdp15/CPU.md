# DEC PDP-15 CPU

SLeeLa profile for the DEC PDP-15 family, a later 18-bit member of the PDP lineage.

- **Status:** profile-driven functional foundation; not a cycle-accurate emulator.
- **Word model:** 18-bit words.
- **Architecture:** accumulator-oriented instruction execution with memory, arithmetic/logical, control, and I/O operations.
- **Model differences:** PDP-15 installations and options may differ; memory capacity, extended instructions, and peripherals are configured by profile.
- **Compatibility:** separate PDP-15 decoding and behavior from PDP-4/PDP-7/PDP-9 and from the 12-bit PDP-5/PDP-8.

Only features declared by the selected hardware profile are enabled. Unsupported operations generate diagnostics instead of being silently accepted.