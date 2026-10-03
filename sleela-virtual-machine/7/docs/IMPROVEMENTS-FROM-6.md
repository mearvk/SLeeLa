# Improvements from SLVM/6 to SLVM/7

| Area | /6 | /7 |
|---|---|---|
| Logging | Evidence-chain observer | First-class Log Manager with fault semantics |
| Memory | Resource limit | Managed accounting, pressure, checkpoint awareness |
| Health | Continuous verification | Explicit watchdog and heartbeat manager |
| Recovery | Checkpointed migration | Bounded recovery with quarantine |
| Managers | Distributed components | Required Manager Registry and dependency checks |
| Checkpoints | Migration evidence | Explicit atomic/replay-safe checkpoint contract |
| Resources | Quotas/reservations | Pressure detection and controlled throttling |
| Faults | Fail-closed controls | Normal/degraded/recovery/quarantine lifecycle |
| Attestation | Runtime evidence | Manager-health and recovery-state evidence |
| Migration | Runtime compatibility | Manager-set compatibility and post-transfer revalidation |
