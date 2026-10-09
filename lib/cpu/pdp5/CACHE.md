# PDP-5 Cache Policy

The historical PDP-5 profile does **not** assume a modern CPU cache hierarchy.

- Default: no L1/L2/L3 cache.
- Memory accesses go through the configured core-memory abstraction.
- Optional host-side memoization may be used only as an emulator optimization and must remain invisible to guest execution.
- Any optimization must invalidate on writes and preserve observable memory/I/O ordering.

This document describes emulator policy, not a claim that the PDP-5 included a cache.