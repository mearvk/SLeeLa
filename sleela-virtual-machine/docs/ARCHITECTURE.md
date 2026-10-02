# SLVM Architecture

The SLVM is the execution core of SLeeLa. It is deliberately separated from the existing `runtime/` support layer.

## State

The first implementation contains:

- code buffer
- code size
- program counter
- operand stack
- stack capacity
- halt state

## Instruction Model

Instructions begin with a one-byte opcode. Instructions requiring operands encode those operands immediately after the opcode.

The initial prototype supports:

- NOP / HALT
- CONST / POP / DUP
- ADD / SUB / MUL / DIV / NEG
- EQ / LT / GT
- JUMP / JUMP_IF_FALSE
- RETURN

This is an initial execution kernel, not yet the complete SLeeLa language instruction set.

## Integration

Compiler and loader integration will be added only after the instruction representation is established and tested. Existing `runtime/` services remain reusable dependencies.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
