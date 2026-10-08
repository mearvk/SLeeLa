# Inmos Transputer Architecture

## Execution path

SLTransputerCPU
-> instruction fetch
-> decode
-> operand/workspace evaluation
-> integer/FPU execution
-> process scheduler
-> channel/link interface
-> memory
-> completion

## Concurrency

Unlike conventional single-threaded CPU models, the Transputer architecture explicitly supports process scheduling and communication channels. SLeeLa therefore models scheduler, channel, and link resources as first-class architectural components.

## Profiles

T4xx and T8xx generations differ in memory, links, floating point, and implementation details. Those differences remain profile-specific.
