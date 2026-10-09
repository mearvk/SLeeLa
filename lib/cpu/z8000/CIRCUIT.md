# Zilog Z8000 Circuit Model

## Functional scope

Architectural functional model, not transistor-level reconstruction.

## Blocks

1. Instruction fetch and instruction register.
2. Variable-length decoder.
3. R0-R15 register file.
4. Effective-address generator.
5. Integer/logic execution unit.
6. Condition-code/status logic.
7. Z8001 segment unit where applicable.
8. Memory/I/O interface.
9. Interrupt/control logic.
10. Clock and sequencing.

## Couplers

SLCoupler models fetch-to-decode, decode-to-registers, registers-to-execution, address-to-memory, execution-to-status, interrupt-to-control, and DMA-to-bus-arbitration paths.
