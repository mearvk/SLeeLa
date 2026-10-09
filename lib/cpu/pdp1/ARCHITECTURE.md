# PDP-1 Architecture

The PDP-1 implementation models an 18-bit accumulator-centered machine.

1. Fetch the next instruction from configured memory.
2. Decode with PDP-1-specific field and opcode rules.
3. Execute memory, arithmetic/logical, control-flow, or I/O behavior.
4. Update architectural state and memory in the order defined by the instruction.
5. Dispatch console and peripheral operations through the configured device map.

Keep the decoder, instruction semantics, memory model, and peripheral implementations independent. Unsupported instructions or device operations return structured diagnostics; they must not silently behave as no-ops. The default mode is functional, not cycle-accurate.