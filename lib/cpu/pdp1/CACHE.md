# PDP-1 Cache Policy

The baseline PDP-1 profile assumes **no modern CPU cache hierarchy**.

- Route memory operations through the configured memory abstraction.
- Host-side memoization is allowed only if it is invisible to the guest.
- Invalidate cached host data on writes.
- Preserve instruction-visible memory and I/O ordering.

This is an emulator policy, not a claim about every later system or peripheral configuration.