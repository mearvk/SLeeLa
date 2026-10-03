# SLVM/7 Architecture

SLVM/7 hardens the SLVM/6 execution model with explicit managers and a controlled failure lifecycle.

SLeeLa source -> authoritative compiler -> Output Symbols/Core -> verified artifact -> SLVM/7 -> Manager Registry -> capability/security boundary -> authenticated broker/resolver -> OS adapter.

## Manager Registry

The Manager Registry discovers and validates required runtime managers before execution. At minimum, SLVM/7 expects:

1. Log Manager
2. Memory Manager
3. Health/Watchdog Manager
4. Recovery Manager
5. Checkpoint Manager
6. Resource Manager
7. Attestation Manager
8. Lineage Manager

A manager may observe or coordinate only within its capability boundary. Manager presence never grants authority.

## Failure lifecycle

Normal -> Degraded -> Checkpointing -> Recovering -> Quarantined

A security or integrity failure can bypass Degraded and enter Quarantined. The VM does not silently continue after a required-manager failure.

## Log Manager

Security, recovery, manager-health, checkpoint, and fault events are structured and chained. Security-sensitive logging failure is itself a fault. Secrets are redacted before persistence or export.

## Memory Manager

Memory accounting includes committed, reserved, and checkpoint memory. Pressure is detected before the configured budget is exceeded. The policy is checkpoint-then-throttle where safe; integrity faults quarantine the runtime.

## Health Manager

Heartbeats, checkpoint recency, log sequence progress, and memory pressure are part of runtime health. A stalled VM enters recovery/quarantine according to policy.

## Recovery

Recovery accepts only intact checkpoints whose artifact, policy, lineage, and manager state remain compatible. Recovery attempts are bounded. Exhaustion results in quarantine rather than an uncontrolled retry loop.

## Migration

Migration retains the SLVM/6 requirements and additionally verifies manager-set compatibility and recovery state before and after transfer. Native OS handles are re-established through the destination broker.

## Security principle

**Every manager can report a fault; no manager can unilaterally redefine SLeeLa execution semantics or grant itself authority.**
