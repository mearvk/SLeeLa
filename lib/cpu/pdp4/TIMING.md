# PDP-4 Timing

The baseline profile makes no cycle-accuracy claim.

- **Functional mode:** executes defined instruction semantics deterministically.
- **Timed mode:** consumes explicit per-instruction and, where needed, per-device timing metadata from the selected machine profile.
- **Unknown timing:** report unavailable; do not substitute PDP-5, PDP-8, or later PDP-family timings.

Host execution speed is not guest timing. Timing profiles should identify their source and distinguish measured, documented, and estimated values.