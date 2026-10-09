# PDP-4 DMA and Transfer Policy

The baseline PDP-4 profile does not presume a generic DMA controller.

- CPU-mediated I/O uses the configured device interface.
- A machine-specific transfer extension must be explicitly declared and documented.
- Extensions must define memory ownership, transfer range, completion, errors, and ordering visible to the CPU.
- If no extension is configured, DMA requests return an unsupported-operation result and must not mutate memory.

This conservative default avoids importing transfer hardware from later systems into the PDP-4 model.