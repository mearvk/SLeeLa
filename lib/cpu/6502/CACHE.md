# 6502 Cache

## Cache hierarchy

The original 6502 has no conventional CPU cache hierarchy.

| Level | Present | SLeeLa model |
|---|---|---|
| L1 instruction | No | absent |
| L1 data | No | absent |
| L2 | No | absent |
| L3 | No | absent |

## Timing consequence

A memory access is modeled directly through the processor's bus interface. There is no cache-hit path that bypasses the external memory cycle.

This is important for the SLeeLa timing model: CPU clock cycles and memory/bus cycles remain observable rather than being hidden behind a cache abstraction.

## Variant rule

Do not attach modern cache objects to 6502-derived profiles unless a specific derivative or surrounding processor actually contains such storage.
