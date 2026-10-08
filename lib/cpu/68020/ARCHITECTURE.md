# Motorola 68020 Architecture

## Datapath

SL68020CPU
-> instruction fetch/cache
-> decoder
-> effective-address generation
-> integer/shift execution
-> memory
-> exception/branch
-> completion

## Pipeline

The 68020 uses a pipelined implementation. SLeeLa exposes fetch, decode/effective-address, execute, memory, and completion events without claiming a modern superscalar design.

## Bus

The processor has a 32-bit address bus and 32-bit data bus with dynamic bus sizing for smaller transfers.

## Exceptions

The 68020 exception architecture includes interrupts, traps, bus/address errors, and instruction faults. Vectoring and status-stack behavior remain part of the CPU model.
