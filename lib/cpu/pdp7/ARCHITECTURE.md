# PDP-7 Architecture

The SLeeLa PDP-7 profile uses an 18-bit word model and an accumulator-oriented execution path.

1. Fetch an instruction using the configured PC and memory model.
2. Decode using a PDP-7-specific instruction decoder.
3. Execute memory, arithmetic/logical, control-flow, or I/O operations.
4. Update the accumulator, PC, memory, and any explicitly documented condition/link state.
5. Dispatch peripheral operations to the configured device implementation.

Keep machine-specific decoding separate from common execution helpers. Unsupported encodings and unavailable devices must produce structured diagnostics. Do not use PDP-4 or PDP-9 decode tables as substitutes for a verified PDP-7 definition.