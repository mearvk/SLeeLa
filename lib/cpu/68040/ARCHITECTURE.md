# Motorola 68040 Architecture

## Datapath

SL68040CPU
-> instruction fetch/decode
-> integer execution
-> address generation
-> memory translation
-> I/D caches
-> FPU
-> bus
-> completion

## Superscalar execution

The 68040 can execute more than one instruction during portions of its pipeline. SLeeLa represents this with issue/execution resources without pretending that the 68040 is a modern wide-issue out-of-order CPU.

## Pipeline

The architecture is modeled with fetch, decode, address generation, execution, memory, and write-back/retirement stages.

## FPU

The integrated FPU is a first-class execution resource for MC68040. LC040/EC040 profile differences remain explicit.
