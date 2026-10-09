# PDP-14 Transfer Policy

Generic CPU-style DMA is not assumed for the PDP-14 baseline.

- External signals use the configured I/O provider.
- Bulk or specialized transfer facilities require an explicit hardware extension.
- Any extension must define ownership, point/address range, completion, failure, and ordering.
- Unsupported transfer requests return a structured error and diagnostic.

Do not attach the shared SLeeLa DMA controller by default merely because other CPU profiles use it.