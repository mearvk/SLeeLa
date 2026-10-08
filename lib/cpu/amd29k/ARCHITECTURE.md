# AMD 29000 Architecture

## Execution path

SLAMD29KCPU
-> fetch
-> decode
-> register-window mapping
-> integer execution
-> optional FPU
-> load/store
-> cache/MMU
-> bus
-> completion

## Register windows

The 29000 architecture provides a large logical register space intended to reduce procedure-call save/restore overhead. SLeeLa models the logical register mapping independently from the physical implementation.

## Profiles

29000-family members differ in cache, MMU, FPU, pipeline, and system-interface features. Those characteristics remain profile-specific.
