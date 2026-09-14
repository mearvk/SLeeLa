# Garbage Collector and Security Supervisor

The runtime now provides explicit C and C++ implementations and a Sleela semantic contract.

## Security policy

The supervisor supports class allow/deny rules, per-class instance and byte quotas, and aggregate quotas for explicit policy roles: User, Business, Corporation, LLC, Fiduciary, Contract, Attorney, Senator, Democrat, and Custom.

These labels are organizational policy inputs only. The runtime does not infer human worth or use IQ, morality, biology/specimen, race, religion, disability, or similar personal characteristics to grant or deny software access.

For quality-sensitive admission, use objective and auditable attributes such as code-signing status, provenance, version constraints, capability grants, security-review state, contract authorization, or administrator approval.

## Runtime path

authorize class -> reserve quota -> allocate -> execute -> release -> collect

Unknown classes are denied by default. A denied class or exhausted quota prevents admission.

## Quantity and quality

Quantity is enforced through instance and byte limits. Quality is represented by explicit software policy attributes rather than a score assigned to a person.

## Garbage collection

The C implementation provides a mark/sweep-compatible registry with explicit root marking and child traversal callbacks.

The C++ implementation provides a safe shared-ownership baseline. It is intentionally separated from the eventual precise Sleela heap so tracing can be added without changing the security boundary.
