# Challenge Manager and Reports Manager

## Challenge Manager

The Challenge Manager provides declared, bounded diagnostic challenge probes and canaries. It does not inject arbitrary hidden executable payloads. A challenge may be installed locally or on an explicitly authorized remote SLVM target through existing capability, certificate, security, and resolver controls.

When a declared system state or binary result matches its condition, the manager emits a `ConditionObserved` event. Authorized delivery may use a registered listener, computer identity, IP/port endpoint, or messaging endpoint. Remote delivery is authenticated, capability-scoped, auditable, and resolver-aware.

### Editions

- Basic: local challenge probes and ConditionObserved events.
- Moderate: authorized remote targets, resolver support, and audit.
- Advanced: capability-scoped remote operation, attestation, provenance, and controlled rollback.

## Reports Manager

Reports Manager provides authorized observation of system input, output, messages, and system records. Records are converted into named binary output objects and routed through the IQ/system-output path. Named binary objects are typed data records, not executable payloads.

Authorized records can be forwarded through a messaging API. Stream authorization, object identity, sequencing, destination authorization, and audit policy apply.

## Common flow

System -> capability/security boundary -> challenge or report record -> named binary object -> IQ/output routing -> authorized listener or messaging API.
