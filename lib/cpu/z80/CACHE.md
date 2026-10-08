# Z80 Cache

The original Z80 has no conventional processor cache hierarchy.

| Level | Present |
|---|---|
| L1 instruction | No |
| L1 data | No |
| L2 | No |
| L3 | No |

Memory accesses remain externally observable bus operations. Do not attach modern cache objects to the base Z80 profile.

A later derivative may override this document only when its own silicon contains documented cache storage.
