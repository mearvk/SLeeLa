# Motorola 68060 Architecture

## Datapath

SL68060CPU
-> fetch/decode
-> instruction buffer
-> branch prediction/control
-> dispatch/issue
-> integer execution
-> FPU
-> address generation
-> MMU
-> I/D caches
-> bus
-> completion

## Superscalar execution

The 68060 expands the superscalar design with multiple integer execution paths and more advanced instruction scheduling than the 68040.

## Branch handling

Branch prediction and a dedicated branch-processing path are represented explicitly. The model does not reduce every control transfer to a simple sequential pipeline.

## Pipeline

SLeeLa exposes fetch, decode, dispatch, execute, memory, and completion stages while leaving exact internal cycle timing to the 68060 profile.
