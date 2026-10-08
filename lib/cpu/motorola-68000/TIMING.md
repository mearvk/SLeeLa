# Motorola 68000 Timing

## Clock versus bus timing

The 68000 clock period is not equivalent to instruction latency. Instructions may require multiple internal and external bus phases.

SLeeLa records clock cycle, bus cycle, operation class, address, data, transfer size, read/write, wait/acknowledge, interrupt acknowledge, and bus arbitration.

## Timing classes

The model distinguishes byte and word reads/writes and read-modify-write cycles. Interrupt acknowledge and bus arbitration have their own timing paths.

## Design principle

Do not collapse all instructions into a fixed cycle count. The effective addressing mode, operand size, memory access, and external bus conditions can alter elapsed time.
