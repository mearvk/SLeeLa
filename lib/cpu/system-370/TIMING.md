# System/370 Timing and Performance

## Model-specific behavior

System/370 performance varies significantly by model, configuration, instruction, storage hierarchy, and I/O load. Do not publish one universal cycles-per-instruction value.

## Measurement dimensions

Track instruction execution, storage wait, translation overhead, interruption processing, and channel/device activity independently where the simulator supports them.

## Evidence classes

- `documented`: source data tied to a named model.
- `derived`: calculated from documented values and stated assumptions.
- `estimated`: simulation approximation.
- `unspecified`: no reliable value established.

## Reports

Include model identifier, architecture level, enabled facilities, storage configuration, and evidence class. Keep functional correctness distinct from cycle-accurate timing.
