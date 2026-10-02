# SLVM/4 Architecture

SLVM/4 builds on /3 with authenticated distributed execution and controlled cross-runtime interoperability.

SLeeLa source -> authoritative compiler -> Output Symbols/Core -> verified artifact -> SLVM/4 -> local or remote capability broker -> runtime adapter.

## Distributed Object Model

Objects crossing a process or host boundary receive explicit object IDs and leases. Calls require authenticated transport, freshness protection, authorization, and observer correlation.

## JVM Interoperability

The JVM/object broker is an interoperability boundary, not a second SLeeLa runtime. Java/JavaFX objects may be represented through broker handles and explicit lifecycle operations.

## Capability Delegation

A remote capability delegation is scoped to a capability set, object, peer identity, issuer, and expiration time. Delegation never creates authority that the originating policy does not permit.

## Resolver

DNS/IP/path resolution is an input to policy, not an authority source. Resolver results require the appropriate capability and may require authenticated or trusted provenance. A newer IP or path must never silently bypass capability policy.

## Observability

Every distributed request receives a correlation identity. Observer records cover broker frames, object lifecycle, delegation, resolver activity, certificate decisions, and attestation while retaining /3 secret redaction and tamper-evident chaining.

## Security

Remote execution is opt-in. Missing peer identity, encryption, freshness, lease validity, certificate trust, or capability authorization produces a denial when required by policy.
