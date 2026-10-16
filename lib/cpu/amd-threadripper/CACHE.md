# AMD Ryzen Threadripper 9000 (Zen 5 x86-64) Cache

## Cache hierarchy

| Level | Present | SLeeLa model |
|---|---|---|
| L1 instruction | Yes | present |
| L1 data | Yes | present |
| L2 | Yes (per-core) | present |
| L3 | Yes (shared per CCD) | present |

L3 capacity scales with core/CCD count across the roster: 64 MB (9945WX/9955WX) up to 384 MB (PRO 9995WX). See SPECIFICATION.md for per-SKU figures.

## Timing consequence

The model records the presence of the cache levels above. The functional model accesses memory through the bus interface; cache levels are represented as architectural facts and timing hints rather than a cycle-accurate replacement policy.

## Variant rule

Do not attach cache objects that this part does not actually contain, and do not remove levels it does. A different SKU or derivative with a different cache configuration is a separate profile.

## Sources

- https://en.wikipedia.org/wiki/Zen_5
- https://en.wikipedia.org/wiki/Ryzen_Threadripper
- https://en.wikipedia.org/wiki/List_of_AMD_Ryzen_processors
