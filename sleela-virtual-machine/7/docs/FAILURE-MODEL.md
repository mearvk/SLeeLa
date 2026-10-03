# SLVM/7 Failure Model

SLVM/7 treats failure as a state transition with evidence rather than an exception that disappears after a message.

## States

- **Normal** — required managers healthy and evidence current.
- **Degraded** — non-critical capability is impaired; execution is restricted.
- **Checkpointing** — runtime is producing a recovery boundary.
- **Recovering** — a bounded recovery attempt is active.
- **Quarantined** — execution authority is suspended pending controlled intervention.

## Examples

| Fault | Required response |
|---|---|
| Memory pressure | checkpoint if safe, then throttle |
| Memory integrity failure | quarantine |
| Log sink degradation | retain local evidence; security-log failure may quarantine |
| Watchdog stall | checkpoint/recover if possible; otherwise quarantine |
| Manager dependency failure | prevent startup or quarantine |
| Checkpoint corruption | reject checkpoint; recover from earlier valid checkpoint |
| Resolver provenance loss | reject affected resolution |
| Attestation staleness | reject security-sensitive transition |

No failure path should create an implicit authority escalation.
