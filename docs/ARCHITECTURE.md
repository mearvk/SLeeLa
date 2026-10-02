# SLVM/6 Architecture

SLVM/6 builds on /5 with continuous verification across execution epochs, migration, leases, certificates, and resolver evidence.

## Execution lineage
Each epoch records parent lineage plus artifact, manifest, policy, capability, runtime, resolver, and certificate evidence.

## Migration
A checkpoint is accepted by a target only after integrity, artifact, policy, ABI/runtime compatibility, capability compatibility, and attestation checks. Native OS handles are not copied implicitly.

## Continuous verification
Security-sensitive transitions revalidate policy, capability, certificate, lease, and lineage evidence. Cached authorization cannot outlive its governing policy.

## Resolver continuity
DNS/IP/path provenance follows execution lineage. A resolver result is never an authority grant.
