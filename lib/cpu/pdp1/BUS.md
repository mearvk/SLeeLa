# PDP-1 Memory, Console, and I/O

Use separate interfaces for core memory and I/O devices.

- Memory transfers use the configured PDP-1 word and address model.
- I/O instructions are decoded using PDP-1-specific rules.
- Console, paper-tape, display, and other devices are optional pluggable modules; enable only devices represented in the chosen machine configuration.
- Unknown device operations return structured errors and diagnostic events.
- Maintain deterministic ordering of memory effects and I/O side effects.

Do not treat modern PCI-style buses as native PDP-1 hardware. Peripheral timing is unspecified unless configured explicitly.