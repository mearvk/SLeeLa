# LINC-8 Cache Policy

The baseline LINC-8 profile assumes no modern L1/L2/L3 CPU cache hierarchy.

- Memory accesses pass through the configured memory and mapping abstraction.
- Any host-side optimization must be invisible to both execution personalities.
- Writes and mode changes must invalidate cached decode or translation state where relevant.
- Preserve ordering of memory-mapped or device I/O side effects.

This is emulator policy, not a claim about every possible later modification to a LINC-8 system.