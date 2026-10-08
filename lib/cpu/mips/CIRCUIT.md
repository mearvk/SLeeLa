# MIPS Circuit Model

## Scope

This is a functional CPU circuit model, not a transistor-level reconstruction.

## Major blocks

1. 32-entry general-register file.
2. Constant-zero enforcement for R0.
3. Program counter.
4. Instruction fetch/decode.
5. Immediate/sign/zero extension.
6. ALU and comparator.
7. Shifter.
8. Multiply/divide path with HI/LO.
9. Pipeline stage registers.
10. Load/store address and data paths.
11. Branch/jump control.
12. CP0/system-control path.
13. Exception/interrupt control.
14. External bus interface.
15. Clock/control logic.

## Couplers

SLCoupler models:

- register file -> ALU;
- ALU -> register file;
- register file -> address generation;
- memory -> writeback;
- branch comparator -> PC control;
- CP0 -> exception control;
- pipeline stage -> pipeline stage.

## Accuracy boundary

No particular cache, TLB, FPU, bus protocol, pipeline depth beyond the functional abstraction, or transistor layout is claimed unless supported by a concrete MIPS implementation source.
