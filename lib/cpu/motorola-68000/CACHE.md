# Motorola 68000 Cache

The original MC68000 has no conventional processor cache hierarchy.

| Level | Present |
|---|---|
| L1 instruction | No |
| L1 data | No |
| L2 | No |
| L3 | No |

Every memory operation is modeled through the external bus interface.

Do not attach later 68020/68030/68040/68060 cache structures to this base profile. Those processors require separate CPU models.
