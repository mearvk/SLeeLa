# PDP-5 Bus and I/O

Model core-memory transfers and peripheral I/O separately.

- Memory requests carry address, read/write direction, and a 12-bit data word.
- I/O-transfer instructions dispatch through a configurable device-code map.
- Device behavior is supplied by device modules; unknown devices or functions return a structured unsupported-operation result and emit a diagnostic.
- DMA is not assumed by this baseline profile. A machine-specific peripheral may provide an explicitly configured transfer mechanism.

Do not represent modern PCI-style buses as PDP-5 hardware.