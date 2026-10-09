# DEC PDP-5 CPU

SLeeLa profile for the Digital Equipment Corporation PDP-5, an early 12-bit minicomputer and architectural predecessor of the PDP-8.

- **Status:** documented architectural profile; not a cycle-accurate emulator.
- **Word model:** 12-bit words and a single accumulator with link/carry state.
- **Execution:** fetch/decode/execute, memory-reference operations, operate-class instructions, and I/O-transfer operations.
- **Memory:** profile-configured core-memory capacity; 4K words is the baseline historical configuration, not a universal assumption.
- **Compatibility:** keep PDP-5 rules separate from PDP-8 extensions. Optional features must be selected explicitly.

The implementation records unsupported instructions and device requests through the shared SLeeLa CPU diagnostics interface.