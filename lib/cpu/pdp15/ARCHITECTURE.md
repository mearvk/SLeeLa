# PDP-15 Architecture

Use a PDP-15-specific decode and execution path over an 18-bit word model.

1. Fetch the next instruction through the configured memory interface.
2. Decode opcode, addressing fields, and option-dependent operations.
3. Dispatch to memory, arithmetic/logical, control-flow, or I/O execution.
4. Update accumulator, applicable link/condition state, PC, and memory.
5. Route I/O to configured peripheral modules.

The model profile defines available memory, option sets, device assignments, and timing. Keep these separate from core instruction semantics. Unsupported opcodes or disabled options must return a structured unsupported result and log the instruction context.