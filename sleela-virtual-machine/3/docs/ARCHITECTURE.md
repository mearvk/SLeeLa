# SLVM/3 Architecture

SLVM/3 builds on /2 with stronger artifact verification, isolation, deterministic policy, and tamper-evident observation.

```
SLeeLa source
  -> authoritative compiler
  -> SLeeLa Output Symbols / Core
  -> verified artifact/link boundary
  -> SLVM/3
       |-- memory/security policy
       |-- capability broker
       |-- cryptographic provider
       |-- certificate validation
       |-- isolation quotas
       |-- deterministic policy
       |-- observer/audit chain
       |-- attestation
  -> OS-specific runtime adapter
```

No lower layer may silently redefine an Output Symbol produced by the authoritative compiler.

## Verification

Before execution, an artifact may be required to provide a signed manifest, content hash, ABI identity, VM generation, and declared capabilities. Verification failures are fail-closed when the selected policy requires them.

## Isolation

Isolation policy explicitly limits resources and execution behavior. Resource limits supplement, rather than replace, the capability broker.

## Determinism

Determinism is a policy-controlled execution property. Where the OS is inherently nondeterministic, the runtime must either record the relevant event, use a controlled abstraction, or reject a strict deterministic request.

## Observability

Observer records carry sequence information and a previous-record hash reference so an audit stream can detect modification or deletion. Secret data remains redacted.

## Attestation

Attestation describes the exact runtime generation, artifact identity, policy snapshot, provider identity, and security state used for an execution.

## Compliance

Compliance profiles are technical controls. They can require stronger verification/isolation/audit policies without changing the semantics of the SLeeLa language.
