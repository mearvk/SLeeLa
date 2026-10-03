# SLVM/7 Security

SLVM/7 preserves the fail-closed security model of /6 and makes runtime management itself part of the verified execution contract.

## Required controls

- immutable policy snapshots and per-epoch revalidation
- chained security and recovery logging
- bounded memory with sensitive-memory zeroization
- manager dependency validation
- watchdog-based stall detection
- checkpoint integrity and replay protection
- bounded recovery attempts
- quarantine after unrecoverable required-manager or integrity failure
- resolver provenance continuity
- lease revocation
- attestation of manager health and recovery state

## Failure handling

The VM should prefer a controlled checkpoint, throttle, recovery, or quarantine over continuing with unknown state. A logging failure affecting required security evidence is not treated as an ordinary I/O warning.

Compliance profiles describe technical controls and do not by themselves constitute legal certification.
