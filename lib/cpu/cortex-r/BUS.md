# Cortex-R Bus and Memory Interface

## Configurable memory system

The CPU model uses a platform-supplied memory map and memory-system description. Include code/data regions, TCM, on-chip RAM, flash, external memory, peripheral windows, and reserved regions as required by the target.

## Access validation

Every access is checked for address range, access width, alignment, privilege, memory attributes, and protection rules supported by the selected profile. Device accesses may have side effects and must not be reordered as ordinary memory without architectural/platform permission.

## Latency sources

Model configured TCM timing, cache hit/miss costs, memory-controller wait states, interconnect arbitration, and peripheral response latency. These are implementation and SoC properties, not family-wide constants.

## Ordering and barriers

Implement memory ordering and barrier instructions according to the selected architecture generation and shareable/device-memory attributes. Do not invent ordering guarantees for an unspecified interconnect.

## Platform devices

DMA, interrupt controllers, watchdogs, timers, and safety devices are attached through the platform's documented interfaces. Unmapped or prohibited accesses return the appropriate modeled fault or bus error.
