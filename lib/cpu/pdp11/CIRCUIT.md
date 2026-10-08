# DEC PDP-11 Circuit Model

## Functional scope

This is a functional architectural model, not transistor-level reconstruction.

## Major blocks

1. Instruction register/fetch.
2. Instruction decoder.
3. Register file.
4. Effective-address generator.
5. Integer/logic execution.
6. Condition-code logic.
7. Memory/I/O interface.
8. Interrupt/trap controller.
9. Optional memory-management interface.
10. System-bus interface.
11. Clock/sequencing.

## Couplers

SLCoupler represents:

- fetch -> decoder;
- decoder -> register file;
- register file -> address generation;
- address generation -> bus;
- execution -> condition codes;
- bus -> memory/I/O;
- interrupt -> processor control;
- DMA -> bus arbitration.
