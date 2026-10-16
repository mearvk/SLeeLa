# Atari VCS (x86-64 AMD Ryzen) Cache

## Cache hierarchy

| Level | Present | SLeeLa model |
|---|---|---|
| L1 instruction | Yes | present |
| L1 data | Yes | present |
| L2 | Yes | present |
| L3 | No | absent |

## Timing consequence

The model records the presence of the cache levels above. The functional model accesses memory through the bus interface; cache levels are represented as architectural facts and timing hints rather than a cycle-accurate replacement policy.

## Variant rule

Do not attach cache objects that this part does not actually contain, and do not remove levels it does. A different SKU or derivative with a different cache configuration is a separate profile.

## Sources

- https://en.wikipedia.org/wiki/Atari_VCS_(2021_console)
- https://en.wikipedia.org/wiki/List_of_AMD_processors
