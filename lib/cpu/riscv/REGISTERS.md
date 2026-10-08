# RISC-V Registers

## Integer register file

- x0/zero: hardwired zero
- x1-x31: general-purpose registers
- pc: program counter

RV32I specifies 32 XLEN-wide integer registers and a program counter. citeturn0search0

## Conventional ABI roles

- x1: return address
- x2: stack pointer
- x5: alternate link register
- other registers receive ABI-defined roles, but the ISA does not make those roles mandatory. citeturn0search0

## CSRs

Control and Status Registers are a separate privileged/system register file. It includes architectural state such as exception control, counters, interrupt state, address translation, and extension-specific state.

## FP/vector

F/D extensions add floating-point state; V adds vector state. These files exist only when their extensions are selected.
