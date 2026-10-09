# PDP-9 Memory and I/O

- Memory transfers use 18-bit data words and the configured address range.
- I/O instructions use a PDP-9-specific decoder and configurable device/function map.
- Device modules implement peripheral semantics; machine profiles decide which devices exist.
- Unknown devices or functions produce a diagnostic and structured unsupported-operation result.
- Preserve CPU-visible ordering between memory and I/O side effects.

Do not present PCI-style buses as native PDP-9 hardware. Specialized transfer facilities must be explicit, documented machine-profile extensions.