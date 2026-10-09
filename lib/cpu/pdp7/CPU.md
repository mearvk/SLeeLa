# DEC PDP-7 CPU

SLeeLa profile for the Digital Equipment Corporation PDP-7, a 18-bit word-addressed minicomputer in the later PDP-4 lineage.

- **Status:** profile-driven functional foundation; cycle accuracy is not implied.
- **Word width:** 18 bits.
- **Execution model:** accumulator-centered, stored-program execution with memory-reference, operate, control-flow, and I/O behaviors.
- **Machine configuration:** memory size, addressing details, device map, and timing must be selected per hardware profile.
- **Family isolation:** PDP-7 semantics remain separate from PDP-4, PDP-9, PDP-15, and the 12-bit PDP-5/PDP-8 line.

This implementation provides a clear extension point for verified instruction definitions and peripheral modules; it does not claim a complete emulator.