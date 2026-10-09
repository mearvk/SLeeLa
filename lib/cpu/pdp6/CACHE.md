# PDP-6 Cache Policy

The baseline PDP-6 model does not assume a modern L1/L2/L3 CPU cache hierarchy.

- Route guest memory operations through the configured memory abstraction.
- Host-side memoization is allowed only as an invisible optimization.
- Invalidate cached host data on writes and preserve memory/I/O ordering.
- Do not describe host optimizations as PDP-6 hardware.

Any historically specific memory-buffer or interleaving behavior must be represented separately and only when supported by a selected machine profile.