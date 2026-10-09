# PDP-9 Cache Policy

The baseline PDP-9 model does not assume a modern L1/L2/L3 cache hierarchy.

- CPU memory operations use the configured memory abstraction.
- Host-side caching or memoization is allowed only as an invisible emulator optimization.
- Writes must invalidate affected cached values.
- Preserve deterministic memory and I/O ordering.

This is a software-model policy and should not be interpreted as a claim about every later derivative or peripheral arrangement.