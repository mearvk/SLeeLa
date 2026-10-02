# SLVM/3 Security

SLVM/3 strengthens /2 with verified execution and isolation.

Security requirements:
1. Sensitive operations fail closed when policy requirements cannot be satisfied.
2. Artifacts can require content hashes, signatures, ABI identity, and capability declarations.
3. Runtime policy is snapshotted for attestation and audit.
4. Isolation quotas constrain memory, files, network, resources, steps, and time.
5. Capabilities remain authoritative; quotas never grant access.
6. Cryptography remains provider-backed.
7. Certificate trust decisions are explicit and auditable.
8. Observer records are secret-redacted and tamper-evident.
9. Strict deterministic mode must not silently permit uncontrolled nondeterminism.
10. Attestation describes actual execution state rather than making unsupported certification claims.
