# PDP-1 DMA and Transfers

The baseline PDP-1 profile does not assume a generic DMA controller.

- CPU-mediated device operations use the configured I/O interface.
- Any device-specific transfer extension must be explicitly enabled and documented.
- Extensions must define transfer range, memory ownership, completion, error behavior, and CPU-visible ordering.
- Without an enabled extension, DMA requests return an unsupported-operation result and do not mutate guest memory.

This prevents later-system facilities from being incorrectly attributed to the PDP-1 baseline.