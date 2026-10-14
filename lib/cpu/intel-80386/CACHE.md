# Intel 80386 Cache

## Cache hierarchy

| Level | Present | SLeeLa model |
|---|---|---|
| L1 instruction | No | absent |
| L1 data | No | absent |
| L2 | No | absent |
| L3 | No | absent |

## Timing consequence

A memory access is modeled directly through the processor's bus interface; there is no cache-hit path that bypasses the external memory cycle. CPU and bus cycles remain observable rather than hidden behind a cache abstraction.

## Variant rule

Do not attach cache objects that this part does not actually contain, and do not remove levels it does. A different SKU or derivative with a different cache configuration is a separate profile.

## Sources

- https://en.wikipedia.org/wiki/I386
- https://en.wikipedia.org/wiki/X86
