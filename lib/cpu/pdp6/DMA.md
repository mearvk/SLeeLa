# PDP-6 DMA and Transfer Policy

The baseline PDP-6 profile does not assume a generic DMA controller.

- CPU-mediated I/O uses the configured device interface.
- Device-specific data channels or transfer mechanisms may be modeled only when explicitly described by the selected machine/peripheral profile.
- Any transfer extension must define memory ownership, transfer range, completion, errors, and CPU-visible ordering.
- Without a configured extension, DMA requests return an unsupported-operation result and must not mutate guest memory.

This is a conservative emulator default, not a claim about every possible PDP-6 installation.