# DEC Alpha Architecture

## Datapath

SLAlphaCPU
-> instruction fetch
-> decoder
-> integer/FP register files
-> integer/FP execution
-> branch/control
-> load/store
-> MMU/TLB
-> cache/interconnect
-> retirement

## Fixed instruction format

Alpha uses 32-bit fixed-length instructions. The decoder selects the appropriate integer, memory, branch, floating-point, or PAL/system operation.

## Execution

The generic execution model supports:

- integer ALU;
- shift/byte manipulation;
- integer multiply/divide;
- floating point;
- load/store;
- branch prediction;
- conditional moves;
- atomic memory operations.

## Pipeline

A concrete Alpha implementation selects its pipeline and scheduling strategy. EV6, for example, was an aggressive out-of-order superscalar implementation; that should not be projected backward onto EV4.

## Memory ordering

Alpha's memory model is deliberately represented explicitly through memory-ordering state and synchronization operations rather than being collapsed into a generic sequentially-consistent assumption.

## PAL/system state

Privileged transitions and PALcode calls pass through the system-control block.
