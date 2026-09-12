# SLeeLa HTTP 3.0 Protocol Specification

**Status:** Proposed SLeeLa protocol specification  
**Version:** HTTP 3.0  
**Purpose:** A lean, fast, transport-compatible HTTP application protocol for SLeeLa services.

## 1. Design Principle

SLeeLa HTTP 3.0 is designed to become **faster and leaner**, not merely larger. It uses conventional HTTP infrastructure while optimizing the SLeeLa application boundary.

The protocol separates:

- HTTP transport and network infrastructure.
- SLeeLa service naming and dispatch.
- SLeeLa business logic.
- Java and other client integrations.

Routers, switches, proxies, TLS terminators, fiber, coax, Ethernet, Wi-Fi, and other lower-layer infrastructure do not need to understand SLeeLa.

## 2. Compatibility Position

HTTP 3.0 is an **SLeeLa application protocol**, not a replacement for the Internet's underlying HTTP transport standards. It is intended to operate over ordinary HTTP-compatible deployments and may use HTTP/1.1, HTTP/2, or HTTP/3 transports according to the host environment.

The name HTTP 3.0 here identifies the SLeeLa protocol generation. It must not be confused with the IETF HTTP/3 transport protocol.

## 3. Request Model

A request identifies a SLeeLa service and operation with a compact logical form:

```text
POST /sleela/<service>/<operation>
```

The request body contains the smallest representation necessary for the selected operation.

A future negotiated binary representation may replace the textual body without changing the logical service/operation model.

## 4. Fast Naming

HTTP 3.0 treats service and operation names as performance-critical data.

Implementations SHOULD:

1. Cache resolved service identifiers.
2. Cache operation identifiers.
3. Prefer stable compact identifiers after negotiation.
4. Avoid repeatedly transmitting long names on persistent connections.
5. Fall back to ordinary names when no compact dictionary is available.

Conceptually:

```text
First request:   service="orders", operation="calculate"
Later requests:  service=7, operation=3
```

The numeric values are connection/service-local identifiers and MUST NOT be assumed globally stable.

## 5. Compact Packing

HTTP 3.0 defines a compact application envelope for SLeeLa values.

A logical envelope contains:

```text
VERSION | FLAGS | SERVICE-ID | OP-ID | REQUEST-ID | PAYLOAD
```

Implementations SHOULD minimize:

- Copies.
- Allocations.
- Repeated parsing.
- Repeated name resolution.
- Unnecessary serialization.

The wire representation MAY be textual for interoperability or binary for negotiated high-performance operation.

## 6. Request IDs

Every request SHOULD have a request identifier when multiplexing, asynchronous operation, retries, or observability are enabled.

The identifier allows a response to be associated with its originating operation without relying on ordering.

## 7. Response Model

A response contains a status and, where appropriate, a SLeeLa result:

```text
STATUS | REQUEST-ID | RESULT
```

The status distinguishes transport acceptance from application execution.

An HTTP success response MUST NOT be interpreted automatically as successful SLeeLa business execution; the application result remains authoritative.

## 8. Persistent Connections

Implementations SHOULD reuse connections where supported by the underlying HTTP transport.

Connection reuse reduces:

- Handshakes.
- Name resolution work.
- Allocation overhead.
- Latency.

The protocol SHOULD therefore be designed around repeated calls over a stable connector rather than one connection per operation.

## 9. Idempotency and Retry

An operation SHOULD declare whether it is safe to retry.

Recommended classes:

```text
READ       — safe to retry when semantics permit.
IDEMPOTENT — repeat produces the same intended state.
MUTATING   — retry requires explicit policy or idempotency key.
STREAM     — retry semantics are operation-specific.
```

Clients MUST NOT blindly repeat an unknown mutating operation after an uncertain transport failure.

## 10. Streaming

HTTP 3.0 MAY support streaming responses and requests where the selected HTTP transport permits them.

Streaming operations SHOULD use request identifiers and explicit completion semantics so that partial transport completion is distinguishable from complete SLeeLa execution.

## 11. Compression

Implementations MAY negotiate content compression. Compression SHOULD be used when it reduces total transfer cost and SHOULD NOT be assumed beneficial for already compact payloads.

The protocol prioritizes compact representation before compression:

```text
compact naming
      ↓
compact packing
      ↓
optional compression
      ↓
HTTP transport
```

## 12. Security

HTTP 3.0 implementations SHOULD use TLS for untrusted networks and SHOULD authenticate callers when services expose privileged or state-changing operations.

Implementations MUST validate:

- Service identifiers.
- Operation identifiers.
- Payload lengths.
- Request IDs.
- Negotiated capabilities.
- Resource limits.

A service MUST NOT expose arbitrary code execution merely because it exposes an HTTP endpoint.

## 13. Resource Limits

Servers SHOULD enforce bounded limits for:

- Request body size.
- Response size where practical.
- Concurrent requests.
- Streaming duration.
- Header size.
- Operation execution time.
- Per-client resource consumption.

Limits SHOULD be explicit and observable.

## 14. Capability Negotiation

A client and server MAY negotiate:

- Protocol generation.
- Binary/text representation.
- Compact name dictionaries.
- Compression.
- Streaming.
- Maximum envelope size.
- Optional SLeeLa extensions.

A peer MUST fall back to a mutually supported representation rather than assuming that every HTTP 3.0 feature is available.

## 15. Java Integration

The SLeeLa Java connector is a first-class HTTP 3.0 client integration surface.

```text
Java UI / Java program
          |
 SleelaJavaConnector
          |
   SleelaHttpConnector
          |
       HTTP 3.0
          |
      SLeeLa service
```

JavaFX, Swing, Java web applications, and other Java programs can therefore consume SLeeLa without implementing SLeeLa business logic themselves.

## 16. Relationship to RMI

HTTP 3.0 and SLeeLa RMI are complementary transports.

RMI is particularly natural for Java-native remote-object integration. HTTP 3.0 is particularly natural for Internet-facing and web-driven integration.

Both should expose the same logical SLeeLa operation model where practical:

```text
                SLeeLa
                   |
          Service / Operation
             /           \
           RMI          HTTP 3.0
             \           /
              Java Connector
```

## 17. Administration and Monitoring

Administrative operations SHOULD be separated from ordinary business operations.

Recommended capabilities include:

- Health.
- Readiness.
- Service metadata.
- Active request counts.
- Latency statistics.
- Error counts.
- Connection state.
- Graceful drain/shutdown.

Administrative endpoints MUST be access-controlled in production deployments.

## 18. Version Progression

SLeeLa HTTP generations should follow a measurable efficiency rule:

> **Each generation should reduce the cost of naming, packing, parsing, transport, and dispatch where compatibility permits.**

HTTP 3.0 therefore establishes compact identifiers and a compact envelope as first-class concepts rather than treating optimization as an afterthought.

Future generations may add more efficient representations while retaining the logical service/operation model.

## 19. Reference Processing Pipeline

```text
HTTP receive
    ↓
minimal parse
    ↓
service-id lookup
    ↓
operation-id lookup
    ↓
compact unpack
    ↓
SLeeLa dispatch
    ↓
business logic
    ↓
compact result pack
    ↓
HTTP response
```

The target is a short, predictable hot path with reusable state and minimal copying.

## 20. Governing Principle

**SLeeLa HTTP 3.0 should be recognizable to ordinary Internet infrastructure, efficient for SLeeLa services, and simple enough that a Java, C, C++, or other client can implement the connector without becoming a SLeeLa runtime.**
