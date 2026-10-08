# IBM POWER Registers

## General

- GPR0-GPR31: 32-bit general-purpose registers
- FPR0-FPR31: 64-bit floating-point registers
- CR: condition register
- LR: link register
- CTR: count register
- XER: fixed-point exception register
- MQ: multiply-quotient register for POWER-family implementations

IBM identifies MQ as POWER-family state and notes that it is not generally part of later PowerPC architecture, with limited historical exceptions. citeturn0search0turn0search10

## Timer

POWER/POWER2 expose processor timing through special-purpose timer state; SLeeLa models this as a timing service rather than assuming modern time-base semantics. citeturn0search2turn0search12
