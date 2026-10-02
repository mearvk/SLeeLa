# SLVM/5 Security

SLVM/5 preserves /4 fail-closed distributed security and adds evidence and recovery controls.

1. Policy snapshots are versioned and hashed.
2. Sensitive execution identifies its policy snapshot.
3. Build and artifact identity are part of attestation.
4. Distributed transactions carry explicit context.
5. Checkpoints require integrity validation before restoration.
6. Recovery cannot restore an invalid or stale checkpoint.
7. Certificate rotation is policy-controlled.
8. Capability and resolver decisions remain explicit.
9. Durable observers retain correlation without exposing secret values.
10. Resource reservations and quotas constrain recovery and distributed workloads.
