# x86 / x86-64 CPU

SLeeLa's x86 profile models the IA-32 and Intel 64 / AMD64 architectural families without pretending that one microarchitecture represents every x86 processor.

Intel's current Software Developer Manuals separately document architecture, instruction encoding, system programming, and model-specific registers. AMD64 likewise extends the legacy programming model with 64-bit registers and additional GPRs. citeturn0search0turn0search24

## Modes

The model recognizes:

- legacy 16/32-bit execution;
- protected mode;
- compatibility mode;
- 64-bit mode / long mode.

AMD documents long mode as containing 64-bit and compatibility submodes. citeturn0search27

## Programmer model

The base legacy register family includes AX/EAX, BX/EBX, CX/ECX, DX/EDX, SI/ESI, DI/EDI, BP/EBP, SP/ESP, IP/EIP, and FLAGS/EFLAGS.

64-bit mode expands the GPRs to 64 bits and adds R8-R15. citeturn0search28

## SLeeLa composition

SLX86CPU composes instruction fetch/decode, register file, ALU, address-generation unit, branch/control logic, floating/vector execution as implementation features, paging/MMU/TLB, cache hierarchy, interrupt control, system-control registers, bus/interconnect, and couplers.

## Variant rule

8086, 80286, 80386, 80486, Pentium, P6, NetBurst, Core, Zen, and later x86 processors must remain distinct implementations where pipeline, cache, execution width, paging, vector extensions, or other microarchitectural behavior differs.
