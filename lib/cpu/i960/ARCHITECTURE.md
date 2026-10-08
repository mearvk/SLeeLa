# Intel i960 Architecture

## Execution path

SLI960CPU
-> instruction fetch
-> decode
-> register/cache management
-> integer execution
-> optional floating-point execution
-> load/store
-> MMU/cache
-> bus
-> completion

## Register organization

SLeeLa represents the i960 register-cache/register-window mechanism as an architectural resource rather than replacing it with a generic flat-register abstraction.

## Profiles

KA/KB/CA/CF and related implementations are separate profiles. MMU, FPU, cache, pipeline, and protection characteristics are not projected universally across the family.
