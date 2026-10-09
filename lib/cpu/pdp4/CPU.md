# DEC PDP-4 CPU

SLeeLa profile for the Digital Equipment Corporation PDP-4, an early 18-bit minicomputer.

- **Status:** profile-driven functional foundation; not a cycle-accurate emulator.
- **Word size:** 18 bits.
- **Design:** accumulator-oriented, stored-program computer with memory and I/O instruction classes.
- **Configuration:** memory capacity, address interpretation, peripheral map, and timing are machine-profile settings.
- **Compatibility:** keep PDP-4 instruction semantics separate from PDP-7/PDP-9/PDP-15 and 12-bit PDP-5/PDP-8 families.

Where a hardware detail varies by configuration or is not confirmed by the selected reference, the profile marks it unspecified rather than inventing behavior.