# Intel 4004 Timing

The baseline SLeeLa 4004 model does not claim cycle accuracy without a validated timing profile.

- **Functional mode:** deterministic instruction semantics and correct instruction-byte consumption.
- **Timed mode:** per-instruction timing metadata from a documented 4004 source.
- Two-byte instructions must account for both instruction bytes and the correct PC advance.
- Chipset access timing may be modeled separately when reliable data is available.
- Missing timing data is reported as unavailable; do not infer timing from later Intel processors.

Host execution speed is not guest processor timing.