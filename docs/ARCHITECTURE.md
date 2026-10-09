# SLVM/6 Architecture

## Status and implementation boundary

This page describes the intended SLVM/6 architecture. A design statement in this
document is not, by itself, evidence that the complete behavior is implemented
or enabled in every build. Check the relevant source tree and automated tests
before relying on a capability in production.

- **Authoritative native implementation:** [`impl/`](../impl/) (C/C++ front end,
  execution core, and native subject integration).
- **Language grammar and version gate:** [`SLEELA.syntax`](../SLEELA.syntax) and
  [`impl/frontend/version.h`](../impl/frontend/version.h) /
  [`impl/frontend/version.cpp`](../impl/frontend/version.cpp).
- **VM generation references:** [`sleela-virtual-machine/`](../sleela-virtual-machine/).
  The presence of a generation-specific specification does not prove that all
  its described runtime operations are implemented.
- **Build and test guidance:** [`docs/README.md`](README.md) and the root
  [`TESTING.md`](../markdown/TESTING.md), plus live
  [GitHub Actions results](https://github.com/mearvk/SLeeLa/actions).

## Execution architecture

SLeeLa source -> authoritative compiler -> Output Symbols/Core -> verified
artifact -> SLVM/6 -> capability/security boundary -> authenticated
broker/resolver -> OS adapter.

## Execution lineage

The design records parent lineage plus artifact, manifest, policy, capability,
runtime, resolver, and certificate evidence for each execution epoch. Treat
these as required evidence fields only where the corresponding implementation
and validation tests are present.

## Migration

A checkpoint should be accepted by a target only after integrity, artifact,
policy, ABI/runtime compatibility, capability compatibility, and attestation
checks. Native OS handles must not be copied implicitly. Verify the exact
migration path and failure behavior before describing migration as operational.

## Continuous verification

Security-sensitive transitions are intended to revalidate policy, capability,
certificate, lease, and lineage evidence. A test should demonstrate both
successful validation and rejection of missing, stale, or invalid evidence.

## Resolver continuity

DNS/IP/path provenance may follow execution lineage, but a resolver result is
never itself an authority grant. Routing and resolver inputs must not bypass
capability checks or security policy.

## Current validation caveat

The latest inspected Linux and Windows subject-test runs reported invalid-pointer
crashes in the physics, economics, inference, and finance fixtures. These failures
remain unresolved by this documentation update. Do not characterize the entire
test suite as passing until a newer CI run confirms it.
