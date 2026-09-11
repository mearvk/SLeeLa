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

## 3. Layered Architecture

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

## 4. Connector Contract

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

## 5. Typed Value Model

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

## 6. Invocation Envelope

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

## 7. Result Envelope

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

## 8. Request Correlation

Every 2.0 invocation should have a request identifier when the selected transport supports it.

The identifier permits:

- Client/server correlation.
- Log correlation.
- Distributed tracing integration.
- Retry analysis.
- Operational diagnosis.
- Multi-step workflow tracking.

Request identifiers must not be interpreted as authentication credentials.

## 9. Capability Negotiation

A 2.0 service may advertise capabilities such as:

```text
TEXT-ARGUMENTS
TYPED-VALUES
ASYNC-INVOCATION
STREAMING
HEALTH
METRICS
VERSION-2
TLS
AUTHENTICATION
```

A client must not assume a capability merely because the server supports RMI 2.0. The capability must be available through the selected endpoint or explicitly configured by the deployment.

## 10. Version Negotiation

A client should identify the connector protocol version it understands.

A server should reject unsupported mandatory protocol requirements cleanly rather than silently interpreting incompatible data.

A deployment may support:

```text
2.0 client <-> 2.0 server
2.0 client <-> 1.0-compatible server
1.0 client <-> 2.0 server in compatibility mode
```

The compatibility rules should be explicit. Silent semantic downgrades are discouraged.

## 11. RMI/JRMP Profile

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

## 12. HTTP/HTTPS Profile

HTTP remains a first-class 2.0 transport for web-driven artifacts.

A gateway may expose:

```text
GET  /sleela/health
GET  /sleela/capabilities
POST /sleela/invoke
```

HTTPS should be preferred across untrusted networks.

The logical invocation and result envelopes remain the same even when the serialization changes from Java RMI objects to HTTP payloads.

## 13. Local Process Profile

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

## 14. Physical Medium Independence

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

## 15. Router and Network Appliance Compatibility

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

## 16. Security Architecture

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

## 17. Resource and Abuse Controls

A 2.0 service should be able to establish limits such as:

- Maximum request size.
- Maximum argument depth.
- Maximum execution time.
- Maximum concurrent requests.
- Maximum response size.
- Maximum stream duration.
- Maximum connection count.

These limits protect both the Java connector and the SLeeLa service from accidental or hostile resource consumption.

## 18. Health and Observability

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

## 19. Idempotency and Retry

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

## 20. Asynchronous Operations

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

## 21. Streaming

Streaming is an optional capability rather than a mandatory feature.

Potential applications include:

- Large data transfer.
- Event feeds.
- Progress information.
- Log streams.
- Long-running computation output.

A service must explicitly advertise streaming support before a client relies on it.

## 22. Java UI Integration

JavaFX and Swing applications are first-class Java hosts for RMI 2.0.

```text
                  SLeeLa
                     |
            SleelaJavaConnector
                 /       \
             JavaFX     Swing
```

Neither UI toolkit needs to understand the underlying RMI, HTTP, or process protocol.

## 23. Web-Driven Integration

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

## 24. Compatibility Rule

RMI 2.0 must preserve the 1.0 conceptual model:

- Standard lower-layer protocols remain standard.
- Physical media remain independent.
- Routers remain ordinary routers.
- Java remains the integration host where Java is selected.
- SLeeLa remains authoritative for SLeeLa operations.

Enhancement must occur without requiring every network component to become SLeeLa-aware.

## 25. Reference Implementation Direction

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

## 26. Version 2.0 Design Statement

SLeeLa RMI 2.0 is therefore a **transport-neutral service contract layered over conventional communications infrastructure**.

It recognizes the practical reality of modern systems:

```text
Application
   -> Connector
   -> Standard Transport
   -> Standard Network
   -> Standard Physical Medium
```

SLeeLa adds authority at the application layer rather than attempting to replace the infrastructure beneath it.
