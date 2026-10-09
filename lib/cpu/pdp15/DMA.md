# PDP-15 DMA and Transfer Policy

Do not assume a generic DMA feature solely from the CPU family name.

- CPU-driven I/O uses the configured peripheral interface.
- Controller-specific data transfers must be enabled as explicit hardware extensions.
- Each extension defines address range, ownership, completion signaling, error handling, and ordering.
- An unconfigured DMA request returns an unsupported-operation result and must not mutate guest memory.

This policy allows machine-specific peripheral configurations without inventing universal PDP-15 DMA semantics.