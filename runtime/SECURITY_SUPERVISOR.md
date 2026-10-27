# Garbage Collector and Security Supervisor

The runtime provides native C policy enforcement and a SLeeLa-facing facade. The facade currently fails closed because the VM-native binding is not yet connected; it must not be described as end-to-end admission enforcement until integration tests prove that allocations reserve quota and all destruction/failure paths release it.

## Security policy

The supervisor supports class allow/deny rules, per-class instance and byte quotas, and aggregate quotas for explicit policy roles: User, Business, Corporation, LLC, Fiduciary, Contract, Attorney, Senator, Democrat, and Custom.

These labels are organizational policy inputs only. The runtime does not infer human worth or use IQ, morality, biology/specimen, race, religion, disability, or similar personal characteristics to grant or deny software access.

For quality-sensitive admission, use objective and auditable attributes such as code-signing status, provenance, version constraints, capability grants, security-review state, contract authorization, or administrator approval.

## Runtime path

authorize class -> reserve quota -> allocate -> execute -> release -> collect

The C supervisor rejects unknown classes by default. The SLeeLa facade's default methods deny admission until an authoritative native binding is installed. Do not use the facade's local counters as evidence that native quotas are enforced.

Unknown classes are denied by default. A denied class or exhausted quota prevents admission.

## Quantity and quality

Quantity is enforced through instance and byte limits. Quality is represented by explicit software policy attributes rather than a score assigned to a person.

## Garbage collection

The C implementation provides a mark/sweep-compatible registry with explicit root marking and child traversal callbacks.

The C++ implementation provides a safe shared-ownership baseline. It is intentionally separated from the eventual precise Sleela heap so tracing can be added without changing the security boundary.
