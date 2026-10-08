# Fairchild Clipper Architecture

## Execution path

SLClipperCPU
-> instruction fetch
-> decode
-> register read
-> integer/branch execution
-> optional floating-point execution
-> load/store
-> memory management
-> cache
-> system bus
-> completion

## Architectural separation

The ISA is kept distinct from the implementation microarchitecture. Pipeline depth, cache geometry, MMU details, and execution resources are profile-specific.
