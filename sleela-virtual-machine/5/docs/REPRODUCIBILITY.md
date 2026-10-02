# SLVM/5 Reproducibility

SLVM/5 records enough identity to compare an execution environment without claiming that every operating-system interaction is inherently deterministic.

Evidence may include:
- VM generation and build identity;
- artifact and manifest hashes;
- compiler/toolchain identity;
- runtime adapter identity;
- crypto provider identity;
- policy version/hash;
- certificate policy;
- capability policy;
- resolver provenance;
- checkpoint sequence.

Deterministic replay is therefore a policy-controlled evidence feature, not a promise that external systems are deterministic.
