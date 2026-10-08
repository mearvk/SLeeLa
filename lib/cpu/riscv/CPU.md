# RISC-V CPU

SLeeLa's RISC-V implementation models the modular RISC-V ISA rather than pretending there is one universal RISC-V microarchitecture.

## Profiles

- RV32I / RV32E
- RV64I
- standard extensions M, A, F, D, C, Zicsr, Zifencei
- vector and other extensions through explicit extension objects
- privileged execution and virtual memory where implemented

RV32I defines 32 integer registers, x0 hardwired to zero, and pc. RV64I widens XLEN to 64 bits. citeturn0search0turn0search1

## Composition

SLRISCVCPU composes instruction fetch/decode, register file, integer execution, branch unit, load/store unit, optional multiply/divide, atomic, FP, vector units, CSR state, MMU/TLB, cache/interconnect, interrupts, and DMA/system-bus coupling.

## Variant rule

A concrete RISC-V core selects its XLEN, ISA extensions, pipeline, cache hierarchy, MMU, privilege modes, branch prediction, issue width, and interconnect. Those properties are never invented by the generic ISA model.
