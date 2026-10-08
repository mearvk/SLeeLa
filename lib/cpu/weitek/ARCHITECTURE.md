# Weitek POWER Architecture

## Execution path

SLWeitekPowerCPU
-> fetch
-> decode
-> integer/floating-point dispatch
-> execution
-> load/store
-> cache/memory
-> system interconnect
-> completion

## Architectural boundary

Weitek POWER is modeled independently from IBM POWER and PowerPC. Shared naming does not imply ISA or microarchitecture identity.

Pipeline depth, cache geometry, MMU behavior, and execution resources remain implementation-specific.
