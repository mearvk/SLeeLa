# PDP-7 DMA and Transfer Policy

The baseline profile does not presume a generic DMA controller.

- CPU-mediated I/O uses the configured peripheral interface.
- Device-specific transfers can be implemented as opt-in extensions only when supported by the selected machine configuration.
- Each extension must specify address range, memory ownership, completion, error handling, and CPU-visible ordering.
- An unconfigured DMA request must return an unsupported-operation result and must not alter guest memory.

Do not inherit transfer features from later PDP-family systems by default.