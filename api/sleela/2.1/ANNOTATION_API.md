# SLeeLa Annotation API

## Purpose

The Annotation API connects architecture documents to source responsibilities, extension routes, ownership, and production operations.

Architecture: Document → Annotation → Scope → Area → Responsibility → Route → Provider → Source → Execution
Operations: Execution → Stage → Group → Counter → Runbook → Evidence

## Defined annotations

| Annotation | Meaning |
|---|---|
| @scope | Architectural scope |
| @area | Subsystem or expansion area |
| @next | Architectural continuation |
| @responsibility | Primary responsibility |
| @provider | Provider boundary |
| @source | Concrete source boundary |
| @requires | Dependency or prerequisite |
| @capability | Capability or authorization requirement |
| @group | Responsible engineering or operational group |
| @counter | Named operational measurement |
| @stage | Lifecycle or deployment stage |
| @release | Release or release-line association |
| @runbook | Operational procedure reference |

## @next contract

Canonical form: @next <scope>/<area>/<responsibility>.
@next is non-executable metadata. It does not load source, execute a route, grant capability, authorize deployment, or select a runtime implementation.

## Production trace contract

Recommended minimum: @scope, @area, @responsibility, @source, @requires, @capability when applicable, @group, @counter, @stage, @release, @runbook, and @next when a continuation exists.

## SourceRouter relationship

Annotations describe architecture. SourceRouter resolves runtime routing. Annotation consumers must not silently become a second routing engine. Any operational consumer remains subordinate to SourceRouter contracts, capabilities, security policy, dependency rules, build constraints, and deployment authorization.

## Group and counter semantics

@group is an ownership identifier and does not grant repository, deployment, or runtime permission.
@counter is a measurement identifier and does not create the metric implementation. Counter implementations remain responsible for storage, aggregation, publication, retention, and concurrency behavior.

Recommended stable counter names include route.resolve.success, route.resolve.failure, transport.connection.active, and transport.packet.rejected.

## Stage and release

@stage communicates lifecycle context such as development, test, staging, or production. @release associates the responsibility with a release line. Neither is a deployment command or authorization token.

Max Rupplin — MEARVK LLC — 2026