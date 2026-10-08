# x86 / x86-64 Cache

Cache is highly implementation-dependent across x86 generations.

SLeeLa therefore models cache as a concrete CPU implementation component:

- L1 instruction cache;
- L1 data cache;
- optional L2;
- optional L3;
- translation caches/TLBs;
- paging-structure caches;
- coherency state.

Intel explicitly documents TLB and paging-structure caching as part of the memory-translation system. citeturn0search26

No generic cache size, associativity, line size, replacement policy, or inclusive/exclusive relationship is assigned to all x86 processors.

A 486, Pentium, Core, and Zen implementation should each provide its own cache profile.
