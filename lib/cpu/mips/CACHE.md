# MIPS Cache

Cache is an implementation property, not a universal property of the MIPS instruction-set architecture.

The base SLMIPSCPU therefore exposes cache coupling points without asserting a size, associativity, line size, or number of levels.

A concrete MIPS implementation may attach:

- instruction cache;
- data cache;
- unified cache;
- L2/L3;
- scratchpad/closely coupled memory.

Modern MIPS processor documentation demonstrates substantial implementation diversity, including cores with instruction/data caches and larger coherent L2 systems. citeturn0search2turn0search10

No cache values are invented for the base profile.
