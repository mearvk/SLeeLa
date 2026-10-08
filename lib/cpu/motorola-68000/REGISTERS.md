# Motorola 68000 Registers

| Register | Width | Category |
|---|---:|---|
| D0-D7 | 32 | data |
| A0-A7 | 32 | address/stack |
| PC | 32 | control |
| SR | 16 | status/control |

## Status register

SR contains the condition-code register plus supervisor and interrupt-mask control. SLeeLa should preserve the distinction between programmer-visible status and internal control state.

## Stack pointer

A7 is the active stack pointer. Supervisor/user stack behavior is represented at the architecture layer where applicable.

## Internal state

Internal instruction, operand, address, and bus latches are not promoted to architectural registers unless a timing or circuit model requires them.
