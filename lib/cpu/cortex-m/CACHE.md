# Cortex-M Cache Model

## Family rule

Caches are optional and implementation-specific. Many Cortex-M profiles have no architecturally visible data/instruction cache; some high-performance implementations, notably selected M7-class designs and later implementations, may include caches. Never infer cache presence or capacity from the family label alone.

## Configuration

The target description may define:
- instruction cache presence, line size, associativity and capacity;
- data cache presence, line size, associativity and capacity;
- write policy and allocation behavior;
- maintenance operations, barriers, and cacheability attributes;
- memory regions that are non-cacheable or device memory.

Only accept values grounded in the chosen implementation documentation or clearly labeled estimates.

## Behavior

If no cache is configured, memory requests bypass the cache model. If configured, cache hits/misses affect modeled latency and visibility as the implementation and memory attributes specify. Cache maintenance instructions and barriers must be gated by profile and implementation support.

## Coherency

Do not assume hardware coherency between CPU caches and DMA-capable peripherals. The SoC integration may require explicit clean/invalidate operations and memory barriers. Document the platform policy.

## Reporting

Unknown line size, replacement policy, or latency remains `unspecified`; do not fill gaps with generic values.
