# SLeeLa RMI 2.0 Specification

## 1. Purpose

SLeeLa RMI 2.0 builds on the conservative interoperability model of RMI 1.0 and adds a stronger **SLeeLa-aware connector architecture**.

Version 2.0 is intended for systems in which SLeeLa is not merely a remote executable, but an authoritative service participating in desktop, server, web, and networked application environments.

The governing principle is:

> **Standard transport, explicit connector, typed SLeeLa authority.**

RMI 2.0 does not replace ordinary Internet protocols. It makes the SLeeLa application boundary more explicit and more capable while remaining transport-independent.

## 2. Relationship to RMI 1.0

RMI 1.0 establishes:

- Conventional Java RMI/JRMP use.
- TCP/IP transport assumptions.
- Physical-medium independence.
- Local process integration.
- HTTP/web integration.
- Narrow remote contracts.
- Textual baseline arguments and results.

RMI 2.0 retains those principles and adds:

- Typed value envelopes.
- Capability identification.
- Request identifiers.
- Correlation metadata.
- Structured status information.
- Version negotiation.
- Explicit connector capabilities.
- Better lifecycle semantics.
- Transport-neutral observability.
- More deliberate security policy.
- Compact packing and reduced dispatch overhead.
- Faster service-name resolution through explicit identity and caching.

## 3. Lean-by-Version Principle

Every later SLeeLa RMI version should become **faster and leaner in its implementation path**, even when it becomes richer semantically.

The optimization target is:

```text
faster name
   -> faster pack
   -> faster transport
   -> faster unpack
   -> faster dispatch
```

Additional features must not automatically mean additional mandatory work. Optional capabilities should be negotiated rather than imposed on every request.

RMI 2.0 should therefore favor:

- Compact field representations.
- Length-delimited values instead of repeated parsing.
- Direct operation identifiers after service negotiation.
- Cached service identity and endpoint information.
- Reusable connections where the transport permits it.
- Bounded allocation.
- Fewer intermediate object conversions.
- Zero-copy or low-copy transfer where practical.
- Fast-path handling for common scalar values.
- Optional metadata rather than mandatory metadata on every message.

The richer 2.0 semantic model must not require a heavier implementation for simple operations.

## 4. Layered Architecture

```text
+--------------------------------------------------------+
| SLeeLa Authority / Business Semantics                  |
+--------------------------------------------------------+
| SLeeLa RMI 2.0 Service Contract                        |
+--------------------------------------------------------+
| SLeeLa Java Connector                                 |
+--------------------------------------------------------+
| RMI/JRMP | HTTP/HTTPS | Local Process | Future        |
+--------------------------------------------------------+
| TCP/IP / OS Process Boundary                           |
+--------------------------------------------------------+
| Ethernet / Wi-Fi / Fiber / Coax / Other IP Medium     |
+--------------------------------------------------------+
| Physical Layer                                         |
+--------------------------------------------------------+
```

The physical medium remains irrelevant to the SLeeLa application contract.

## 5. Connector Contract

The common Java integration surface is represented by:

```text
SleelaJavaConnector
SleelaInvocation
SleelaResult
```

An application should be able to select a transport without rewriting its business-facing integration code.

```java
SleelaResult result = connector.invoke(
        new SleelaInvocation("calculate", "42"));
```

The connector may be implemented over local process execution, Java RMI, HTTP, or another approved transport.

## 6. Fast Naming and Service Identity

RMI 2.0 should separate **human-readable names** from **fast dispatch identity**.

The preferred sequence is:

```text
service name
    -> endpoint/service identity
    -> cached compact service identifier
    -> operation identifier
    -> dispatch
```

The full service name remains available for configuration, diagnostics, authorization, and interoperability. Once a trusted service identity has been established, repeated requests should not need to repeatedly compare or transmit the longest possible name.

Where the transport and deployment permit it, a compact service identifier and compact operation identifier may be negotiated and reused for the lifetime of a session or cached endpoint binding.

Identifiers are opaque protocol values; they are not security credentials and must not be accepted as proof of authorization.

## 7. Typed Value Model

RMI 2.0 introduces a logical typed-value envelope without requiring Java serialization as the universal wire format.

A future-compatible value family may include:

```text
NULL
BOOLEAN
INTEGER
DECIMAL
STRING
BYTES
LIST
MAP
OBJECT-REFERENCE
```

The wire representation must identify the value type explicitly when a typed mode is negotiated.

The textual 1.0 representation remains a valid compatibility mode.

## 8. Compact Packing

RMI 2.0 should provide a compact representation for negotiated typed mode.

The compact representation should favor:

- Small type tags.
- Length prefixes appropriate to the value size.
- Compact integer encodings where practical.
- Direct byte sequences for `BYTES`.
- Sequential list/map encoding without redundant field names.
- Explicit framing so the receiver can skip or reject an invalid value quickly.

A conceptual packed request is:

```text
[version][service-id][operation-id][flags][arguments]
```

A conceptual packed result is:

```text
[version][request-id][status][value-or-error]
```

The exact binary encoding is an implementation-profile decision until a wire-format profile is standardized. Implementations must not claim wire-level interoperability merely because they use the same logical field names.

The design goal is simple: **pack once, send once, unpack once, dispatch immediately.**

## 9. Invocation Envelope

A 2.0 invocation should be logically representable as:

```text
version
requestId
service
operation
arguments
capabilities
metadata
```

Example:

```text
version   = 2.0
requestId = 8f2d...
service   = accounting
operation = calculate
arguments = [42]
```

The exact serialization may differ between RMI, HTTP, and future transports, but the semantic fields remain stable.

## 10. Result Envelope

A 2.0 result should distinguish outcome from transport.

```text
version
requestId
status
value
error
metadata
```

A conceptual successful result is:

```text
status = OK
value  = 1764
```

A conceptual application failure is:

```text
status = APPLICATION_ERROR
error  = "invalid account state"
```

A transport failure remains a connector/transport event rather than being represented as an SLeeLa business decision.

## 11. Request Correlation

Every 2.0 invocation should have a request identifier when the selected transport supports it.

The identifier permits:

- Client/server correlation.
- Log correlation.
- Distributed tracing integration.
- Retry analysis.
- Operational diagnosis.
- Multi-step workflow tracking.

Request identifiers must not be interpreted as authentication credentials.

## 12. Capability Negotiation

A 2.0 service may advertise capabilities such as:

```text
TEXT-ARGUMENTS
TYPED-VALUES
COMPACT-PACKING
COMPACT-NAMES
ASYNC-INVOCATION
STREAMING
HEALTH
METRICS
VERSION-2
TLS
AUTHENTICATION
```

A client must not assume a capability merely because the server supports RMI 2.0. The capability must be available through the selected endpoint or explicitly configured by the deployment.

## 13. Version Negotiation

A client should identify the connector protocol version it understands.

A server should reject unsupported mandatory protocol requirements cleanly rather than silently interpreting incompatible data.

A deployment may support:

```text
2.0 client <-> 2.0 server
2.0 client <-> 1.0-compatible server
1.0 client <-> 2.0 server in compatibility mode
```

The compatibility rules should be explicit. Silent semantic downgrades are discouraged.

Future versions should preserve the rule that a version increase is not permission to make the common path slower without necessity.

## 14. RMI/JRMP Profile

The RMI transport remains based on standard Java RMI mechanisms where RMI is selected.

RMI 2.0 does not require replacing the Java registry or inventing a new Java transport protocol.

The improvement is primarily at the SLeeLa service contract and connector layer.

```text
JavaFX / Swing / Java Server
            |
    SleelaJavaConnector
            |
      SleelaRmiConnector
            |
       Java RMI/JRMP
            |
       SLeeLa Service
```

The connector should keep the hot path small: established endpoint, compact identity, direct operation dispatch.

## 15. HTTP/HTTPS Profile

HTTP remains a first-class 2.0 transport for web-driven artifacts.

A gateway may expose:

```text
GET  /sleela/health
GET  /sleela/capabilities
POST /sleela/invoke
```

HTTPS should be preferred across untrusted networks.

The logical invocation and result envelopes remain the same even when the serialization changes from Java RMI objects to HTTP payloads.

For repeated calls, HTTP connection reuse and compact negotiated payloads should be preferred where supported by the HTTP implementation.

## 16. Local Process Profile

A local process connector remains valid in 2.0.

This allows a Java desktop application to use the same logical invocation contract even when no network is present.

```text
Java Application
       |
Connector API
       |
Local Process
       |
SLeeLa
```

Local process implementations should avoid unnecessary shell interpretation, repeated executable discovery, and repeated conversion of already-encoded arguments.

## 17. Physical Medium Independence

RMI 2.0 remains independent of physical networking technology.

The same service contract may operate across:

- Fiber.
- Coax.
- Twisted-pair Ethernet.
- Wi-Fi.
- Cellular networks.
- Satellite links.
- Virtual interfaces.
- Loopback.

Optical, electrical, and radio engineering remain lower-layer responsibilities.

## 18. Router and Network Appliance Compatibility

A router, firewall, NAT gateway, switch, or other conventional network appliance should not need SLeeLa-specific forwarding logic for ordinary deployments.

SLeeLa traffic should remain ordinary traffic for the selected transport protocol.

A network appliance may enforce policy based on conventional properties such as:

- Source and destination addresses.
- Source and destination ports.
- Protocol.
- TLS policy.
- Firewall rules.
- Routing policy.

Application-aware inspection may exist, but it is not required for protocol correctness.

## 19. Security Architecture

RMI 2.0 treats security as an explicit service property.

The implementation should support a deployment policy covering:

- Endpoint authentication.
- Authorization of operations.
- Transport confidentiality where required.
- Integrity protection.
- Replay considerations.
- Service identity.
- Capability restrictions.
- Audit logging.
- Rate and resource controls.

A capability advertisement is not authorization. The server must independently enforce authorization.

Compact identifiers must not weaken this rule. A short name or numeric identifier is an index, not a permission.

## 20. Resource and Abuse Controls

A 2.0 service should be able to establish limits such as:

- Maximum request size.
- Maximum argument depth.
- Maximum execution time.
- Maximum concurrent requests.
- Maximum response size.
- Maximum stream duration.
- Maximum connection count.

These limits protect both the Java connector and the SLeeLa service from accidental or hostile resource consumption.

## 21. Health and Observability

Health is a first-class operational concern.

A service should be able to report at least:

```text
service identity
protocol version
health state
available capabilities
```

Optional observability may include:

- Request counts.
- Failure counts.
- Latency.
- Active request counts.
- Transport status.
- Service startup time.

Observability data must not disclose sensitive business information merely for diagnostic convenience.

## 22. Idempotency and Retry

RMI 2.0 should explicitly identify whether an operation is safe to retry.

A request identifier can support correlation, but it does not automatically make an operation idempotent.

Operations should therefore be classified where practical as:

```text
READ_ONLY
IDEMPOTENT
NON_IDEMPOTENT
UNKNOWN
```

Clients should not automatically retry unknown or non-idempotent operations after uncertain transport failures.

## 23. Asynchronous Operations

A future 2.0-compatible extension may support asynchronous invocation.

The conceptual model is:

```text
submit -> requestId
          |
          +--> execution
          |
          +--> completion/result
```

Asynchronous support must not change the meaning of the ordinary synchronous connector call.

## 24. Streaming

Streaming is an optional capability rather than a mandatory feature.

Potential applications include:

- Large data transfer.
- Event feeds.
- Progress information.
- Log streams.
- Long-running computation output.

A service must explicitly advertise streaming support before a client relies on it.

## 25. Java UI Integration

JavaFX and Swing applications are first-class Java hosts for RMI 2.0.

```text
                  SLeeLa
                     |
            SleelaJavaConnector
                 /       \
             JavaFX     Swing
```

Neither UI toolkit needs to understand the underlying RMI, HTTP, or process protocol.

## 26. Web-Driven Integration

A web-driven artifact can use HTTP while a Java desktop application uses RMI or a local process connector.

All can share the same logical SLeeLa operation model.

```text
Browser ---- HTTP -----+
                       |
JavaFX ----- RMI ------+--> SLeeLa
                       |
Swing ------ Process --+
```

This is a principal objective of RMI 2.0: **one SLeeLa authority, multiple conventional integration paths.**

## 27. Compatibility Rule

RMI 2.0 must preserve the 1.0 conceptual model:

- Standard lower-layer protocols remain standard.
- Physical media remain independent.
- Routers remain ordinary routers.
- Java remains the integration host where Java is selected.
- SLeeLa remains authoritative for SLeeLa operations.

Enhancement must occur without requiring every network component to become SLeeLa-aware.

At the same time, each successive version should improve the common path through less packing overhead, faster naming, fewer copies, and more direct dispatch.

## 28. Reference Implementation Direction

The SLeeLa repository should treat the following as the reference implementation layers:

```text
connector/
  core common connector contracts
  process local process adapter
  rmi Java RMI adapter
  http HTTP connector and gateway

rmi/
  Java RMI service and server lifecycle

gui/
  Java desktop runtime integration
```

The implementation should keep these layers separable so that adding HTTP, JavaFX, Swing, or future transports does not alter SLeeLa business semantics.

The implementation should also measure the hot path independently:

```text
name resolution
packing
transport handoff
unpacking
dispatch
```

A performance regression in one stage should not be hidden by aggregate latency alone.

## 29. Version 2.0 Design Statement

SLeeLa RMI 2.0 is therefore a **transport-neutral service contract layered over conventional communications infrastructure**, with an explicit requirement that additional capability be accompanied by a leaner execution path wherever practical.

It recognizes the practical reality of modern systems:

```text
Application
   -> Connector
   -> Standard Transport
   -> Standard Network
   -> Standard Physical Medium
```

SLeeLa adds authority at the application layer rather than attempting to replace the infrastructure beneath it.

The performance direction is equally explicit:

> **Every version should make the common operation easier to name, faster to pack, faster to move, faster to unpack, and faster to dispatch.**
