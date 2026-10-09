# PDP-12 Timing

The baseline profile does not claim cycle accuracy.

- **Functional mode:** deterministic instruction behavior for supported instructions.
- **Timed mode:** uses explicit timing metadata for the selected machine revision, instruction environment, and device.
- **Unknown timing:** report unavailable rather than borrowing PDP-8 or LINC timing values without evidence.
- Mode transitions and I/O completion timing must be defined by the active profile if simulated.

Host runtime speed is not guest timing. Label timing values as documented, measured, estimated, or unspecified.