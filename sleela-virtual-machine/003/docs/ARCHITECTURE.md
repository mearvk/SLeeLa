# SLVM/3 Architecture

SLVM/3 builds on /2 with stronger artifact verification, isolation, deterministic policy, and tamper-evident observation.

SLeeLa source -> authoritative compiler -> SLeeLa Output Symbols/Core -> verified artifact/link boundary -> SLVM/3 -> capability broker -> OS-specific runtime adapter.

Verification may require signed manifests, content hashes, ABI identity, VM generation, and declared capabilities. Required failures are fail-closed.

Isolation explicitly limits resources and execution behavior. Quotas supplement, rather than replace, the capability broker.

Deterministic execution is policy-controlled. OS nondeterminism must be recorded, abstracted, or rejected when strict deterministic policy requires it.

Observer records include sequence and previous-record hash references so audit streams can detect modification or deletion. Secrets remain redacted.

Attestation identifies the actual VM generation, artifact, policy snapshot, provider, isolation state, and runtime adapter. It is evidence about execution state, not a claim of legal certification.
