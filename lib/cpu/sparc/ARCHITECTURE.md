# SPARC Architecture

## Datapath

SLSparcCPU
-> instruction fetch
-> decoder
-> register-window file
-> integer unit
-> branch/control
-> load/store
-> optional FPU
-> MMU/TLB
-> cache/interconnect
-> trap/retirement control

## Register windows

SAVE and RESTORE change the active window state. Window spill/fill traps are modeled through the trap controller rather than hidden as ordinary register moves.

## PC and nPC

SPARC's control-flow model explicitly represents both PC and next-PC state. Delayed control transfers are therefore represented in the architecture rather than approximated as ordinary branches.

## Privilege/traps

SPARC V9 separates privileged and nonprivileged execution and provides a structured trap state. citeturn0search20

## Implementation boundary

Pipeline depth, issue width, branch prediction, cache topology, FPU implementation, and multithreading remain implementation-specific.
