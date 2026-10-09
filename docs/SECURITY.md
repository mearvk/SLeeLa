# SLVM/6 Security

## Scope and implementation status

SLVM/6 security documentation describes the intended control model. A statement
here is not a certification and does not independently establish that a control
is implemented, active in a particular build, or independently tested. Verify
each security-sensitive path in code and CI before making deployment claims.

SLVM/6 is intended to preserve /5 fail-closed controls and add continuous
verification. The design calls for migration checks over checkpoint, artifact,
policy, runtime, capability, certificate, and attestation evidence; epoch-based
lease revocation; resolver provenance tied to execution lineage; independently
verifiable observer evidence; and failure-domain isolation.

## Required verification evidence

For each control, record the implementation location and a reproducible test
that exercises both the allowed path and the denial path:

- **Integrity:** tampered or incomplete artifacts/manifests must be rejected.
- **Policy and capabilities:** missing, expired, or insufficient authority must
  not be treated as permission.
- **Certificates and leases:** invalid, expired, or revoked evidence must fail
  closed.
- **Migration:** incompatible ABI/runtime state and implicit transfer of native
  OS handles must be rejected.
- **Resolver provenance:** DNS/IP/path results must not grant authority or
  bypass policy.
- **Failure isolation:** a failed remote component must not silently become a
  trusted authority source.

## Current CI caveat

The latest inspected Linux and Windows subject-test runs reported invalid-pointer
crashes in the physics, economics, inference, and finance fixtures. Those are
runtime correctness failures, not proof of a security exploit, but they mean the
overall validation picture is not clean. Check the live
[GitHub Actions results](https://github.com/mearvk/SLeeLa/actions) and do not
claim a green suite until a subsequent run verifies the fixes.

Compliance profiles are technical deployment controls; they do not by themselves
constitute legal certification, third-party audit, or a claim of regulatory
approval.
