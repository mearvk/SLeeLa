# SLVM Architecture

The SLVM is the execution core of SLeeLa and is deliberately separate from `runtime/`.

The initial kernel contains VM state, a program counter, operand stack, opcode decoding, arithmetic/comparison operations, constants, control flow, and halt/return behavior.

The initial instruction set is intentionally a foundation rather than the final SLeeLa instruction set. Compiler/loader integration, call frames, locals, object references, exceptions, and richer values will be added against an explicit instruction-set contract.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
