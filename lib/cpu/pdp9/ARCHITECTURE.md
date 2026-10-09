# PDP-9 Architecture

The PDP-9 model is an 18-bit accumulator-oriented machine profile.

1. Fetch an instruction using the configured PC and memory model.
2. Decode with the PDP-9-specific decoder.
3. Dispatch memory-reference, arithmetic/logical, control-flow, or I/O behavior.
4. Update AC, documented condition/link state, PC, and memory according to the decoded operation.
5. Route peripheral requests through the selected device map.

Keep instruction semantics independent from memory sizing, peripheral configuration, and timing. Unsupported encodings or devices must return structured errors and emit diagnostics. Do not claim compatibility with another PDP family merely because it shares an 18-bit word size.