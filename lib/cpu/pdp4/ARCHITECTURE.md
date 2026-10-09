# PDP-4 Architecture

The PDP-4 profile uses an 18-bit word model and an accumulator-centered execution path.

1. Fetch a word using the configured program-counter and memory-address model.
2. Decode the instruction using a PDP-4-specific decoder.
3. Dispatch to memory, arithmetic/logical, control-flow, or I/O behavior.
4. Update the accumulator, link/condition state, PC, and memory according to the instruction definition.
5. Dispatch I/O through the selected machine's peripheral map.

Keep decode tables, execution semantics, memory configuration, and device models separate. Unsupported encodings must return a structured result and diagnostic rather than silently acting as no-ops. This profile does not claim cycle accuracy or compatibility with other PDP families.