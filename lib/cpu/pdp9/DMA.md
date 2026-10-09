# PDP-9 DMA and Transfer Policy

The baseline PDP-9 profile does not presume a generic DMA controller.

- CPU-mediated I/O uses the configured device interface.
- A transfer mechanism may be enabled only through an explicit, documented machine/peripheral profile.
- Any extension must define memory ownership, address range, completion, errors, and ordering visible to the CPU.
- Without such an extension, DMA requests return unsupported status and must not modify guest memory.

This conservative design avoids silently importing later hardware behavior.