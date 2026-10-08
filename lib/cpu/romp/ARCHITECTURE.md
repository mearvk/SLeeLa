# IBM ROMP Architecture

## Execution path

SLROMPCPU
-> instruction fetch
-> decode
-> register read
-> integer execution
-> branch/control
-> load/store
-> memory management
-> cache/memory
-> system interconnect
-> completion

## Architectural boundary

ROMP is modeled as its own 32-bit RISC architecture. It is not treated as an early POWER or PowerPC implementation.

Pipeline, cache, MMU, and system-controller details remain implementation/platform-specific.
