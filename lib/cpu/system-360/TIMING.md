# System/360 Timing and Performance

## Model-specific timing

There is no single cycle count or execution rate for the System/360 family. Timing depends on the machine model, instruction, operand location, installed options, storage technology, channel activity, and system load.

## Evidence labels

- `documented`: published for a specific model.
- `derived`: calculated from documented data and explicit assumptions.
- `estimated`: approximate simulation value.
- `unspecified`: reliable model-specific evidence is unavailable.

## Performance dimensions

Track instruction execution, storage waits, channel activity, interruption overhead, and device waits separately. Do not include unmodeled operating-system or I/O time as intrinsic CPU latency.

## Reports

Identify the machine model, options, storage configuration, and evidence class. Do not present estimates as guaranteed timing.
