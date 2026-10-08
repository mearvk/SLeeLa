# RISC-V Specification

## Base state

| Resource | RV32I | RV64I |
|---|---:|---:|
| Integer registers | 32 | 32 |
| Register width | 32 | 64 |
| x0 | constant zero | constant zero |
| PC | XLEN | XLEN |

The base ISA does not require dedicated hardware registers for stack pointer or return address; software conventionally uses x2 as SP and x1 as the return-address register. citeturn0search0

## Base encoding

RISC-V keeps rs1, rs2, and rd fields in consistent positions. Base RV32I uses R/I/S/U formats with B/J immediate variants; base instructions are 32 bits. Optional extensions can introduce 16-bit instructions. citeturn0search0

## Extensions

- I: integer base
- M: integer multiply/divide
- A: atomics
- F/D: floating point
- C: compressed instructions
- V: vectors
- Zicsr: CSR instructions
- Zifencei: instruction-fetch synchronization
- additional standard/custom extensions through explicit capability declarations

The standard G profile combines IMAFD with Zicsr and Zifencei. citeturn0search3

## Privilege

Machine, supervisor, and user execution are modeled where implemented; hypervisor and other privileged extensions remain profile-specific.

## Virtual memory

RV64 profiles may select Sv39, Sv48, Sv57, or another defined translation scheme. Sv39 provides a 39-bit virtual address space. citeturn0search24
