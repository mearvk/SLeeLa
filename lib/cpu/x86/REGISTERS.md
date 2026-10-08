# x86 / x86-64 Registers

## General-purpose

Legacy/compatibility:

- AX/EAX
- BX/EBX
- CX/ECX
- DX/EDX
- SI/ESI
- DI/EDI
- BP/EBP
- SP/ESP

64-bit mode adds:

- R8-R15

and exposes 64-bit forms RAX-RSP plus R8-R15. AMD documents the width and register-extension behavior. citeturn0search24turn0search28

## Control-flow

- IP/EIP/RIP
- FLAGS/EFLAGS/RFLAGS

## Segment state

CS, DS, ES, FS, GS, SS.

## Control/debug state

CR0-CR4 and applicable CR8; DR0-DR7; GDTR, IDTR, LDTR, TR.

## Floating/vector state

x87 stack registers, MMX aliases where supported, XMM registers, and wider YMM/ZMM state where the concrete implementation enables SSE/AVX-family extensions.

## Model-specific state

MSRs are implementation-specific and are not flattened into the architectural GPR file. Intel maintains a dedicated MSR manual for these registers. citeturn0search10
