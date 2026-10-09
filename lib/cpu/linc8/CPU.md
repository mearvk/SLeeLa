# DEC LINC-8 CPU

SLeeLa profile for the Digital Equipment Corporation LINC-8, a hybrid computer combining LINC and PDP-8 execution environments.

- **Status:** profile-driven functional foundation; not a cycle-accurate emulator.
- **Word model:** 12-bit machine words.
- **Distinctive behavior:** separate LINC and PDP-8 instruction personalities with explicit mode state.
- **Memory and devices:** configured per machine profile; LINC peripherals must not be treated as generic PDP-8 devices.
- **Compatibility:** use separate decoders and execution paths; do not merge both instruction sets into one ambiguous opcode table.

Exact mode-switch, memory mapping, and peripheral behavior must be specified by a verified LINC-8 machine profile.