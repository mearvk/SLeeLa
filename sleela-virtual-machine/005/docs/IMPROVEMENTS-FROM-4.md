# Improvements from SLVM/4 to SLVM/5

SLVM/5 (**Manager**) advances SLVM/4 (**Supervisor**) from authenticated,
distributed execution toward reproducible, policy-verifiable, fault-tolerant
execution. It remains an execution layer for the authoritative SLeeLa compiler
and Core representation, not a parallel language runtime.

| Area | /4 | /5 |
|---|---|---|
| Reproducibility | Distributed execution id | Reproducible execution + build identity |
| Policy | Node admission policy | Policy versioning + immutable policy snapshots |
| Attestation | Per-node attestation | Stronger attestation evidence |
| Audit | Execution membership | Durable audit checkpoints |
| Transactions | Quorum agreement | Distributed transaction boundaries |
| Recovery | Node re-admission | Explicit fault and recovery state |
| Resources | Quorum sizing | Resource reservation and quotas |
| Capabilities | Node grants | Capability delegation chains |
| Certificates | Node identity | Certificate lifecycle + trust rotation |
| Replay | — | Deterministic replay evidence |
| Compatibility | Single generation | Negotiation across VM/runtime generations |

The policy snapshot validator (`src/slvm5_policy.c`) admits a program only
against an immutable, versioned policy; a malformed snapshot is rejected under
`--selfcheck`.

SLVM/5 remains an execution evolution, not a second language implementation.
