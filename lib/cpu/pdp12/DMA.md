# PDP-12 DMA and Transfer Policy

The baseline PDP-12 profile does not assume a universal DMA controller.

- CPU-mediated I/O uses the active environment's device interface.
- Specialized transfers are modeled only when the configured machine/peripheral profile supports them.
- Any transfer extension must define address bounds, memory ownership, completion, error handling, and CPU-visible ordering.
- Unsupported transfer requests must not silently alter memory.

This conservative model avoids importing unverified transfer hardware into the combined PDP-8/LINC system profile.