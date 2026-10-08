# x86 / x86-64 Specification

## General-purpose registers

Legacy/compatibility programming exposes eight primary GPRs with multiple operand widths. In 64-bit mode the GPRs are 64-bit and R8-R15 are added. citeturn0search24turn0search28

| Resource | Legacy | 64-bit mode |
|---|---:|---:|
| Primary GPRs | 8 | 16 |
| GPR width | up to 32-bit | 64-bit |
| Instruction pointer | IP/EIP | RIP |
| Flags | FLAGS/EFLAGS | RFLAGS |
| Segment registers | present | architectural but largely reduced in 64-bit code/data use |

## Instruction encoding

x86 instructions are variable length. Encoding may include prefixes, opcode bytes, ModR/M, SIB, displacement, and immediate fields. Intel documents instruction encoding and complete instruction references in Volume 2. citeturn0search6

SLeeLa therefore uses a variable-length decoder rather than the fixed-width decoder used by MIPS and PowerPC.

## Execution

The architectural model includes:

- integer arithmetic and logic;
- shifts/rotates;
- control transfer;
- stack operations;
- string operations;
- memory operations;
- privileged/system instructions;
- x87/SSE/AVX families when enabled by the concrete profile;
- exceptions and interrupts.

## Segmentation and paging

Legacy x86 provides segmentation. Modern 64-bit mode substantially changes segment-limit behavior, while paging supplies linear-to-physical translation. AMD documents these 64-bit segmentation changes, and Intel documents paging structures and TLB caching. citeturn0search25turn0search26

## Exceptions

The system model includes faults, traps, aborts, hardware interrupts, software interrupts, descriptor-table state, privilege checks, and interrupt delivery.

## MSRs / control registers

CR0-CR4, CR8 where applicable, debug registers, descriptor-table registers, MSRs, and model-specific state are represented separately from the general register file.
