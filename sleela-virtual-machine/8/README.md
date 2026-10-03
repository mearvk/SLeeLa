<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 8

SLVM/8 is the supervised-execution successor to SLVM/7. It keeps SLVM/7's manager, logging, memory, health, recovery, checkpoint, resource, attestation, lineage, and migration controls, then adds an explicit execution supervisor and admission path.

## Design direction

SLVM/8 moves the architecture from management contracts toward a controlled execution lifecycle:

SLeeLa source -> authoritative compiler -> verified artifact -> admission -> policy/capability validation -> transactional execution -> supervised runtime -> checkpoint/recovery -> broker/resolver -> OS adapter.

Principal additions:
- Execution Supervisor for lifecycle coordination.
- Policy Manager for immutable execution policy.
- Capability Lease Manager for bounded and revocable grants.
- Transaction Manager for explicit begin/commit/abort action groups.
- Audit Manager for chained security evidence.
- Admission Manager for pre-execution checks.
- Deterministic fault handling.

## Security invariant

No execution action crosses the capability/OS boundary until admission succeeds. No capability survives terminal quarantine. No transaction commits after a failed integrity decision.

SLVM/8 remains a common SLeeLa VM contract, not a separate language implementation.

Copyright (c) Max Rupplin - MEARVK LLC - 2026