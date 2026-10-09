# PDP-6 Architecture

The PDP-6 profile represents a 36-bit machine with a PDP-6-specific decoder and register/memory model.

1. Fetch the instruction word using the configured PC and memory model.
2. Decode opcode, addressing fields, and any halfword semantics according to the PDP-6 instruction definition.
3. Execute memory, arithmetic/logical, control-transfer, or I/O operations.
4. Apply 36-bit arithmetic and the documented overflow/link/condition rules where applicable.
5. Route peripheral operations through the configured device interface.

Keep instruction definitions separate from execution handlers. PDP-10 compatibility should be a separately named profile or compatibility layer, not an implicit alias. Unsupported encodings must produce diagnostics and a structured result.