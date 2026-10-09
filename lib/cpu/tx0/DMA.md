# TX-0 DMA and Transfer Policy

The baseline TX-0 profile does not presume a generic DMA controller.

- CPU-mediated transfers use the configured memory and device interfaces.
- Any specialized transfer mechanism must be explicitly documented by the selected machine profile.
- Extensions must define memory ownership, bounds, completion, errors, and CPU-visible ordering.
- Unconfigured DMA requests return an unsupported-operation result and must not modify guest memory.

Do not import later minicomputer transfer hardware into the TX-0 model by default.