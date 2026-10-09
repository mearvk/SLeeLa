# TX-0 Architecture

Use a dedicated TX-0 execution path.

1. Fetch an instruction using the configured PC and memory model.
2. Decode with the TX-0-specific instruction table.
3. Execute the instruction's defined arithmetic, logical, memory, control, or I/O action.
4. Update only the state defined by that instruction and the selected machine profile.
5. Dispatch console/peripheral operations through the device interface.

Keep machine configuration, decode tables, state transitions, and device behavior independent. Unknown instructions must return a structured unsupported result with a diagnostic. Do not borrow PDP-1 or PDP-4 behavior merely because the machines share historical context.