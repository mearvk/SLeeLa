# PDP-7 Cache Policy

The baseline PDP-7 profile assumes **no modern CPU cache hierarchy**.

- Route guest memory accesses through the configured memory abstraction.
- Permit host-side memoization only when guest-observable behavior remains unchanged.
- Invalidate optimized entries on writes.
- Preserve deterministic memory and I/O ordering.

This describes the emulator's implementation policy and does not assert undocumented details about peripheral controllers or later related systems.