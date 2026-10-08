# IBM POWER Cache

Cache organization is implementation-specific.

The SLeeLa model exposes:

- instruction cache;
- data cache;
- optional unified cache;
- cache controller;
- memory interface;
- coherency/interconnect boundary.

POWER implementations vary. For example, IBM documents a single-chip RSC implementation with an 8 KiB combined instruction/data cache and later POWER1 systems with separate instruction and data caches. citeturn0search19

No single cache geometry is imposed on generic POWER.
