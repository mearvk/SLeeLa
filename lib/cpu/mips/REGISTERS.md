# MIPS Registers

## General registers

R0-R31 are 32-bit general-purpose registers.

| Register | Architectural role |
|---|---|
| R0 | constant zero |
| R1-R31 | general purpose; conventional ABI names may assign roles |

## Special registers

HI and LO hold multiply/divide results in classic MIPS designs.

PC holds the current instruction-stream address.

## CP0

CP0 provides privileged system-control registers. The exact register map varies by MIPS generation and implementation. The SLeeLa model therefore exposes a typed CP0 interface rather than pretending every MIPS processor has one identical CP0 map.

MIPS boot documentation identifies CP0 Status and exception-related registers as part of processor initialization. citeturn0search19

## FPU

A floating-point unit is not assumed in the base CPU object. MIPS implementations may provide coprocessor 1; that belongs in a concrete variant.
