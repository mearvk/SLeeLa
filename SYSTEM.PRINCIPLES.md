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
