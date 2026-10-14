# SLVM/8 Architecture

SLVM/8 takes SLVM/7's management layer and makes it an explicit supervised execution pipeline.

## Admission before execution
The Admission Manager must establish artifact validity, immutable policy validity, required-manager health, resource availability, capability validity, attestation validity, and lineage validity. Only then can the Supervisor move from normal to admitted and then running.

## Execution Supervisor
The Supervisor coordinates lifecycle transitions. It does not create language semantics and cannot grant capabilities.

Normal -> Admitted -> Running -> Degraded/Checkpointing/Quiescing -> Recovering -> Running, or -> Quarantined -> Stopped.

Integrity failure bypasses recovery and enters quarantine.

## Policy and capability control
Policy is explicit, versioned, immutable for an execution epoch, and checked before admission. Capabilities are leases with issuance and expiry epochs. Revocation is immediate.

## Transaction Manager
Runtime action groups may commit only while admission and integrity remain valid. The transaction layer does not claim that arbitrary OS operations are magically reversible; non-reversible operations remain broker-policy controlled.

## Audit Manager
Security-relevant decisions produce chained audit records. Audit evidence is separate from diagnostics and does not itself grant authority.

## Relationship to SLVM/7
SLVM/7 established managers and safe failure handling. SLVM/8 adds the missing orchestration layer:

SLVM/7 managers -> Admission -> Policy/Capability validation -> Supervisor -> Transactional execution -> Audit -> Recovery/Quarantine.

## Central rule
Observe, validate, admit, execute, audit, recover or quarantine. Do not execute first and validate later.
