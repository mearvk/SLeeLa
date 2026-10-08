# SuperH Architecture

## Processing

SLSuperHCPU
-> fetch
-> decode
-> register/control state
-> integer execution
-> branch
-> load/store
-> optional DSP/FPU
-> MMU/TLB
-> cache
-> bus

## Register organization

R0-R15 provide the primary general register set. Later implementations add shadow registers and expanded control state.

## Pipeline

SH-1/SH-2/SH-3 profiles remain implementation-specific. SH-4 exposes a five-stage pipeline and superscalar execution; the generic SLeeLa model therefore makes pipeline width configurable by profile. citeturn1search20

## Branching

Delayed branches are an architectural feature of the family. The fetch/control model explicitly retains the delayed-control-transfer state.
