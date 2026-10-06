# SLeeLa Document Annotations

## Purpose

Document-level annotations describe architectural placement, continuation, implementation ownership, and production traceability. They are metadata: they do not execute routing, grant authority, bypass security, or replace source contracts.

## Canonical chains

Architecture: Document → Annotation → Scope → Area → Responsibility → Route → Provider → Source → Execution
Operations: Execution → Stage → Group → Counter → Runbook → Evidence

## Defined annotations

### @scope
Identifies architectural scope. Example: @scope system.

### @area
Identifies subsystem or expansion area. Example: @area network.

### @next
Identifies the next architectural destination. Canonical form: @next <scope>/<area>/<responsibility>. Examples: @next system/network/transport and @next system/program/process. Multiple @next declarations are valid. The destination may be planned rather than implemented. @next is not executable routing.

### @responsibility
Identifies the primary responsibility. Example: @responsibility transport.

### @provider
Identifies the provider boundary. Example: @provider posix-socket.

### @source
Identifies the concrete source boundary. Example: @source impl/network/TcpTransport.cpp.

### @requires
Identifies a dependency or prerequisite. Example: @requires common/buffer.

### @capability
Identifies a capability or authorization requirement. Example: @capability network.connect.

### @group
Identifies the engineering, service, product, or operational group responsible for the responsibility. Example: @group network-runtime. It does not grant permissions.

### @counter
Identifies a stable operational measurement. Examples: @counter route.resolve.success and @counter route.resolve.failure. It does not itself create or publish a metric.

### @stage
Identifies lifecycle context: development, test, staging, or production. Example: @stage production. It is not a deployment command.

### @release
Identifies a release or release line. Example: @release 1.x. It does not authorize deployment.

### @runbook
Identifies the operational procedure. Example: @runbook docs/runbooks/network-transport.md.

## Level 4 production trace

A production-facing responsibility should be traceable without hidden assumptions through: scope, area, responsibility, source, requirements, capability when applicable, group, counters, stage, release, runbook, and next architectural destination.

This gives a developer a direct path from architecture to source, ownership, production measurements, release context, operational procedure, and the next expansion point.

## Interpretation rules

1. An annotation belongs to its declaring document.
2. @next is continuation, not completion.
3. An annotation is descriptive unless a specific tool formally defines operational behavior.
4. Interpretation is deterministic.
5. Unresolved annotations remain visible to diagnostics.
6. Annotations cannot override security, capability, dependency, resource, build, release, or deployment controls.
7. Source headers, interfaces, schemas, security policy, and build rules remain authoritative for exact behavior.
8. Tooling may consume annotations for documentation, source navigation, dependency graphs, ownership views, dashboards, release reports, and future compiler infrastructure.
9. Providers may change while stable responsibilities remain.
10. Group identifies ownership; counter identifies measurement; stage and release identify context; runbook identifies procedure. None of these annotations is itself an authorization or deployment action.

## Complete example

@scope system
@area network
@responsibility transport
@provider posix-socket
@source impl/network/TcpTransport.cpp
@requires common/buffer
@capability network.connect
@group network-runtime
@counter route.resolve.success
@counter transport.connection.active
@stage production
@release 1.x
@runbook docs/runbooks/network-transport.md
@next system/network/multiplexing

## Extension model

Future annotations may include @version, @platform, @protocol, @format, @implements, @extends, @tests, @generates, @deprecated, @experimental, @security, @resource, and @observability. They have no defined semantics until formally documented.

Max Rupplin — MEARVK LLC — 2026