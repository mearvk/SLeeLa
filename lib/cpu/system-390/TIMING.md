# ESA/390 Timing and Performance

## Model-specific timing

ESA/390 is an architecture level implemented by multiple System/390 models. Timing is model-, instruction-, storage-, and configuration-dependent; there is no single universal cycles-per-instruction value.

## Metrics

Where supported, report instruction execution, translation overhead, storage wait, interruption handling, and channel/device activity separately.

## Evidence classes

- `documented`: published for the named model.
- `derived`: computed from documented data and explicit assumptions.
- `estimated`: approximate simulation value.
- `unspecified`: no reliable value established.

## Reporting

Include model, architecture level, addressing mode, facilities, storage configuration, and evidence class. Do not describe a functional simulator as cycle-accurate without validation.
