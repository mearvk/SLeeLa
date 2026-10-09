# z/Architecture Timing and Performance Model

## Timing policy

There is no single reliable cycles-per-instruction figure for the entire z/Architecture family. Performance depends on processor generation, instruction facilities, operand alignment, cache/storage hierarchy, translation, contention, and system configuration.

## Evidence levels

- `documented`: published for a specific processor or system.
- `derived`: computed from documented values and stated assumptions.
- `estimated`: approximate planning model.
- `unspecified`: target evidence unavailable.

## Performance dimensions

Record instruction mix, facility use, branch behavior, storage translation, cache assumptions, synchronization, I/O waits, and multiprocessor contention separately where measurable. CPU instruction timing must not include unmodeled operating-system or channel-subsystem time as if it were intrinsic core latency.

## Reports

Every timing report identifies the processor profile, enabled facilities, system configuration and evidence class. Do not claim exact or guaranteed execution time from the generic family model.
