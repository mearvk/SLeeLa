# DEC PDP-6 CPU

SLeeLa profile for the Digital Equipment Corporation PDP-6, a 36-bit time-sharing computer and an important predecessor of the PDP-10 family.

- **Status:** profile-driven functional foundation; not a cycle-accurate emulator.
- **Word model:** 36-bit words, with instruction formats and halfword behavior defined by the PDP-6 profile.
- **Execution:** memory-reference, arithmetic/logical, control-transfer, and I/O instruction classes.
- **Registers:** configurable architectural register file; exact register and PC semantics must follow a verified PDP-6 reference.
- **Compatibility:** PDP-6 and PDP-10-family similarities do not justify treating their instruction sets as identical.

The implementation isolates word arithmetic, instruction decoding, memory configuration, and I/O device behavior. Unverified fields remain explicitly unspecified.