# Z80 Timing

## M and T cycles

The Z80 timing model is explicitly cycle based:

- T = individual clock period.
- M = machine cycle made from multiple T cycles.
- M1 = normal instruction-fetch cycle.

Zilog documents basic memory and I/O operations as taking multiple T cycles and permits WAIT to lengthen them.

## SLeeLa timing record

cycle = { machine, t, operation, address, data, wait, phase }

This lets the model distinguish clock speed, instruction latency, and bus transaction latency.

## External synchronization

WAIT is represented as an external timing condition rather than an assumed fixed delay. This is important for period-correct emulation and for modeling slower memory/peripherals.
