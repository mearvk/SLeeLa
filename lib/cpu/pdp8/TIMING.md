# PDP-8 Timing and Performance

## Model-specific timing

Timing differs across PDP-8 implementations and options. The architecture profile alone is insufficient to establish exact cycles or elapsed time.

## Metrics

Where data is available, distinguish instruction execution, memory access, I/O wait, interrupt handling, and optional arithmetic-unit activity.

## Evidence classes

- `documented`: timing published for a named model.
- `derived`: calculated from documented values and assumptions.
- `estimated`: approximate simulator value.
- `unspecified`: no reliable model-specific value available.

Do not label this functional model cycle-accurate without comparison against hardware documentation and suitable tests.
