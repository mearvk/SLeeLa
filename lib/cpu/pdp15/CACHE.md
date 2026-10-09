# PDP-15 Cache Policy

The baseline PDP-15 emulator profile does not invent a modern L1/L2/L3 cache hierarchy.

- All guest-visible accesses pass through the configured memory abstraction.
- Host-side memoization is permitted only when behavior remains identical.
- Writes must invalidate affected cached host data.
- Preserve instruction, memory, and I/O ordering.

This is a software-emulation policy; it is not a claim about every peripheral, controller, or later derivative system.