# Motorola 68020 Registers

## Data

- D0-D7: eight 32-bit data registers

## Address

- A0-A7: eight 32-bit address registers
- A7 is the active stack pointer

## Control

- PC: program counter
- SR: status register

The status register contains condition codes and supervisor/interrupt state.

## Additional state

Exception and coprocessor state is represented separately from the programmer-visible D/A register files.
