# TX-0 Memory, Console, and I/O

Keep memory transfers and console/peripheral interactions separate.

- Memory words use the configured 18-bit data model.
- Console and peripheral operations dispatch through the TX-0 device map.
- Device models declare supported operations and completion behavior.
- Unknown device operations produce a diagnostic and structured error.
- Preserve ordering of memory writes and externally visible I/O effects.

Do not represent PCI-style buses or later DEC peripheral conventions as native TX-0 hardware.