# PA-RISC Registers

## General registers

- GR0-GR31
- GR0 is the architecture's zero/constant register in the programmer model.
- Register roles such as stack pointer and return linkage are conventions or instruction-specific uses, not a windowed register architecture.

## Control state

The selected PA-RISC generation provides processor status, protection, interrupt/trap, and translation state.

## Floating point

PA-RISC provides a separate floating-point register file. PA-RISC 1.0 documentation describes 32 64-bit FP registers with alternate 32-bit and 128-bit views. citeturn0search1

## Shadow/fast-interrupt state

PA-RISC 1.0 includes shadow registers for fast interrupt handling. These are modeled separately from the normal GR file. citeturn0search1

## 64-bit transition

PA-RISC 2.0 widens the general registers and functional datapath to 64 bits while maintaining 32-bit compatibility. citeturn0search1
