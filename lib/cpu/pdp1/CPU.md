# DEC PDP-1 CPU

SLeeLa profile for the Digital Equipment Corporation PDP-1, an early interactive 18-bit computer.

- **Status:** profile-driven functional foundation; not a cycle-accurate emulator.
- **Word model:** 18-bit words, with instruction and address fields interpreted by the PDP-1 decoder.
- **Architecture:** accumulator-oriented, stored-program design with arithmetic, memory, control-flow, and I/O operations.
- **Interaction:** I/O devices and console behavior are configured peripherals, not hard-coded assumptions.
- **Compatibility:** keep PDP-1 rules separate from later DEC PDP families.

This profile is intentionally conservative where details depend on the particular installation or a verified programming reference.