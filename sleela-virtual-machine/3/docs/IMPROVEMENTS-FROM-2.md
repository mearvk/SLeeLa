# Improvements from SLVM/2 to SLVM/3

| Area | /2 | /3 |
|---|---|---|
| Runtime | Runtime selector | Validated runtime and policy snapshot |
| Security | Fail-closed policy | Verified execution policy |
| Crypto | Provider abstraction | Provider identity and algorithm policy |
| Linking | Signed manifests | Artifact/hash/ABI/capability verification |
| Certificates | Trust policy | Pinning and stronger trust decisions |
| Observability | Security/crypto/link/cert events | Tamper-evident chained audit records |
| Isolation | Capability boundary | Resource quotas and sandbox profiles |
| Determinism | Policy concept | Explicit deterministic execution policy |
| Attestation | Available/required | Manifest- and policy-bound evidence |
| Compliance | Profiles | Verification/isolation evidence hooks |

SLVM/3 remains an execution evolution, not a second language implementation.
