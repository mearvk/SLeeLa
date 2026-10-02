# SLVM/6 Migration

Migration transfers a validated checkpoint between compatible runtimes. Required checks: checkpoint integrity; artifact and manifest identity; policy compatibility; ABI/runtime compatibility; capability compatibility; certificate/attestation policy; resolver continuity; resource quotas.

Failure is closed when required evidence is stale or absent. OS handles are re-established through the destination capability broker rather than copied implicitly.
