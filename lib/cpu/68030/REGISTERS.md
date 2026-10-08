# Motorola 68030 Registers

## Programmer-visible state

- D0-D7
- A0-A7
- PC
- SR

A7 remains the active stack pointer.

## MMU state

The integrated MMU adds translation/control state including:

- root-pointer registers;
- translation-control state;
- function-code/address-space controls;
- transparent translation blocks.

The MMU supports user/supervisor protection and write protection. citeturn0search12

## Cache state

Cache control and status state is represented separately from the programmer-visible integer registers.
