# SLeeLa System Principles

1. **Route by responsibility.** Route work toward a responsibility rather than growing hard-coded special cases.
2. **Hierarchy is data.** Scope, area, subsystem and implementation target should be explicit routing facts.
3. **Chains are ordered.** Evaluation is deterministic; the first applicable terminal route wins unless a future policy changes that rule.
4. **Source code is the implementation boundary.** Routing selects an implementation boundary; it does not replace the implementation.
5. **Extension before modification.** Prefer new extension points, route nodes, bindings and providers over unrelated core edits.
6. **Network and program/process are sibling areas.** They share routing semantics but retain separate responsibilities.
7. **Composition over monoliths.** Routing, discovery, execution, policy and implementation remain separable.
8. **Explicit context.** Routing decisions should be explainable from explicit context rather than hidden global state.
9. **Stable contracts.** Headers, interfaces, schemas and routing identifiers should remain stable enough for extensions to depend on them.
10. **Fail visibly.** Unresolved, rejected and invalid routes should be observable.
11. **Security follows the route.** Permissions, capabilities, integrity and resource limits must be able to participate before activation.
12. **Build for new areas.** Storage, GUI, database, telephony, HTTP, security, science and tooling should be consumers of the same extensibility pattern.

## Design chain
**System → Domain → Area → Responsibility → Route → Provider → Source → Execution**

This chain is the common expansion language for SLeeLa.

**Max Rupplin — MEARVK LLC — 2026**

13. Documents may declare architectural continuation. Document-level annotations identify scope, area, responsibility, provider, source, dependencies, capabilities, and next destination.
14. @next is metadata, not execution. It describes continuation and may be consumed by documentation, analysis, navigation, or explicitly defined tooling.
15. Operational metadata is subordinate to authoritative controls. Group, counter, stage, release, and runbook annotations improve traceability but cannot override source contracts, security policy, capabilities, dependency rules, build controls, or deployment authorization.
16. Production must be traceable. A production-facing responsibility should be traceable from document to source, responsible group, required capability, stage, release, operational counters, and runbook.

## Annotation and production trace
Architecture: Document → Annotation → Scope → Area → Responsibility → Route → Provider → Source → Execution
Operations: Execution → Stage → Group → Counter → Runbook → Evidence

See ANNOTATION.md and api/ANNOTATION_API.md.

Max Rupplin — MEARVK LLC — 2026
