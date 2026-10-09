# Intel 4004 DMA Policy

The baseline 4004 system model does not presume a generic DMA controller.

- CPU-driven exchanges use the configured external chipset interface.
- Any system-specific transfer facility must be represented as an explicit peripheral extension.
- Extensions must document address/data width, ownership, completion, error reporting, and ordering.
- Unconfigured DMA requests return an unsupported-operation result and must not mutate guest memory.

Do not import modern DMA controller behavior into the original 4004 architecture.