# LINC-8 Timing

The default profile is functional, not cycle-accurate.

- **Functional mode:** deterministic execution of defined instruction semantics.
- **Timed mode:** requires explicit instruction and device timing metadata for the selected machine.
- **Mode transitions:** timing and side effects must be provided by the profile.
- **Unknown timing:** report unavailable; do not borrow timing values from a standalone PDP-8 or standalone LINC without evidence.

Host execution speed must never be presented as guest timing.