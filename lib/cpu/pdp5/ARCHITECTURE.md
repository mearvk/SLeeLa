# PDP-5 Architecture

The PDP-5 profile models a small, accumulator-oriented 12-bit machine.

1. Fetch an instruction from the configured program counter.
2. Decode the instruction class and its address/function fields.
3. Execute memory-reference, operate, or I/O-transfer behavior.
4. Update accumulator, link, program counter, and memory according to the selected instruction.
5. Route I/O requests through the configured device interface.

Keep instruction decoding separate from execution and machine configuration. Memory size, device map, and optional behaviors belong to a profile, not hard-coded global assumptions. PDP-8 extensions are disabled unless independently verified for a target machine.