# SLVM/7 Managers

SLVM/7 makes runtime management explicit so that the VM can identify what it depends upon before execution.

## Core managers

- **Log Manager** — structured, chained, redacted evidence and recovery markers.
- **Memory Manager** — bounded allocation accounting, pressure detection, checkpoint accounting, and sensitive-memory handling.
- **Health Manager** — heartbeat, stall, resource, and checkpoint freshness monitoring.
- **Recovery Manager** — bounded recovery attempts and quarantine.
- **Checkpoint Manager** — atomic, integrity-protected, replay-safe checkpoints.
- **Resource Manager** — CPU, I/O, network, and memory budgets.
- **Attestation Manager** — runtime and manager-set evidence.
- **Lineage Manager** — epoch continuity and provenance.

## Dependency rule

The Manager Registry validates required managers and their dependencies before startup. Optional managers may be absent only when policy permits. A manager cannot declare itself healthy without the required dependency evidence.
