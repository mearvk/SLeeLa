# DEC Alpha Specification

## Programmer model

- 64-bit integer registers R0-R31;
- 64-bit floating-point registers F0-F31;
- fixed 32-bit instructions;
- 64-bit integer/data architecture;
- PC and processor status/control state;
- load/store architecture;
- conditional branches and computed jumps.

Alpha's architectural simplicity deliberately minimized special-purpose integer registers and exposed a regular register-based instruction model.

## Register conventions

R31 is the architectural zero register for integer operations. F31 is the floating-point zero register in the Alpha architecture.

## Instruction classes

- integer operate;
- integer multiply/divide;
- memory load/store;
- branch;
- PAL/system control;
- floating point;
- atomic/conditional memory operations.

## PALcode

PALcode provides a privileged software layer used for low-level processor/system functions. SLeeLa models PALcode as a controlled system interface rather than ordinary application instructions.

## Variants

Alpha 21064/EV4, 21164/EV5, 21264/EV6, EV7, and later designs must remain separate microarchitectural profiles.
