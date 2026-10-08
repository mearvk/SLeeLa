# Motorola ColdFire Architecture

## Datapath

SLColdFireCPU
-> fetch
-> decode
-> effective-address generation
-> integer execution
-> optional MAC/DSP/FPU
-> cache/MMU
-> bus
-> completion

## Embedded design

ColdFire emphasizes a reduced, implementation-efficient 68K-derived instruction architecture. SLeeLa therefore models the ISA compatibility layer separately from the microarchitecture.

## Profiles

V1 through V4e are separate implementation profiles. No later ColdFire pipeline, MMU, cache, or FPU feature is projected backward onto every ColdFire core.
