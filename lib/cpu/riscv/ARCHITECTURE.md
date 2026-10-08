# RISC-V Architecture

## Datapath

SLRISCVCPU
-> instruction fetch
-> decoder
-> register file
-> integer/branch unit
-> optional extension units
-> load/store unit
-> CSR/control
-> MMU/TLB
-> cache/interconnect
-> retirement

## Decode

The decoder recognizes the selected base ISA and extension set. Reserved and unsupported encodings are handled according to the selected execution environment; they are never silently assigned invented semantics.

## Load/store architecture

Only memory-access instructions read or write ordinary memory in the base integer model. Arithmetic operates on registers. RV32I explicitly defines this load-store organization. citeturn0search0

## Control flow

JAL/JALR and conditional branches update PC without an architecturally visible branch delay slot. citeturn0search0

## Extensions

Extension units attach through explicit couplers:
- MUL/DIV
- atomic/AMO
- FP
- vector
- compressed decoder
- CSR/system control

## Microarchitecture

A simple implementation may be single-cycle or multicycle. A high-performance implementation may use pipelining, speculation, prediction, out-of-order execution, and retirement. These are implementation properties, not ISA requirements.
