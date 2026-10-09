# Cortex-R Cache and TCM Model

## Principle

Cache and tightly coupled memory (TCM) features differ across Cortex-R implementations. Presence, capacity, line size, associativity, replacement policy, maintenance operations and TCM layout must come from the selected core documentation.

## Cache behavior

When a cache is absent or disabled, route accesses according to the configured memory system. When enabled, model cacheability attributes, hit/miss behavior, maintenance operations, and latency at the supported fidelity. Avoid claiming exact timing without target data.

## Tightly coupled memory

Represent each TCM region with its address range, access permissions, supported ports, and documented latency. TCM is not interchangeable with cache: it is explicitly mapped memory with implementation-specific integration.

## Predictability and real-time analysis

Report whether code/data accesses use TCM, cacheable memory, or external memory. Cache misses, line fills, write buffers, arbitration, and external wait states can affect worst-case execution time. Do not assume all cache traffic is deterministic.

## DMA and coherency

DMA/cache interaction is platform-dependent. Include documented coherency or software-maintenance requirements; never assume universal hardware coherency.
