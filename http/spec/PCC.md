# HTTP 3.0 Postal Curious Circuit (PCC)

## 1. Purpose

The **Postal Curious Circuit (PCC)** is a registration and provenance layer for the SLeeLa HTTP 3.0 architecture. It allows an author or sender to request that protected packet or flow commitments be registered at authorized routers associated with a country, border, developmental region, or other registered administrative network.

PCC registration is **not packet interception**. A Registered Router records an authenticated observation or forwarding event without receiving the cryptographic material required to decrypt the HTTP/3 application payload.

The design is intended to provide a modern analogue of postal registration: the sender can obtain evidence that a protected communication passed a designated registration point.

## 2. Core Principles

1. PCC is an optional policy layer above HTTP/3 and QUIC.
2. A router registers commitments and routing metadata, not plaintext application payloads.
3. A Registered Router produces a cryptographically verifiable registration receipt.
4. Country, regional, border, and social-system classifications are administrative roles, not cryptographic strength grades.
5. PCC does not create a universal interception key.
6. PCC does not require routers to decrypt and re-encrypt HTTP/3 application traffic.
7. Registration must be monotonic: a later router cannot silently erase an earlier valid registration.
8. The sender chooses the requested PCC registration scope subject to network policy.
9. Sensitive identity data is not inserted merely to make registration convenient.
10. PCC remains compatible with CIC, route maps, route sessions, KDS, and QUIC.

## 3. Registration Model

```text
AUTHOR / SENDER
      |
      | PCC registration request
      v
+-----------------------+
| PCC Circuit Descriptor|
+-----------------------+
      |
      v
  Origin Router
      |
      v
+-----------------------+
| Registered Router     |
| Country / Region      |
+-----------------------+
      |
      +---- Border Router
      |
      +---- Registered Router
      |
      +---- Social-System Router
      |
      v
 Destination / Next PCC
```

The logical circuit is identified independently from individual packets. A packet or flow is represented to the registration layer by a cryptographic commitment.

## 4. PCC Packet Registration

A PCC registration record SHOULD contain:

- PCC circuit identifier;
- registration sequence or epoch;
- packet or flow commitment;
- router identifier;
- router role;
- country or developmental-region policy identifier;
- timestamp;
- previous-registration commitment where chaining is enabled;
- current registration commitment;
- router signature.

The commitment is derived from protected packet metadata and an implementation-defined domain separator. The commitment is not a substitute for HTTP/3 packet encryption.

Conceptually:

```text
packet
  |
  +-- HTTP/3 / QUIC encrypted payload
  |
  +-- PCC commitment
          |
          +-- Circuit ID
          +-- packet/flow commitment
          +-- registration epoch
```

A router signs the observation:

```text
SIGN(
    PCC-ID ||
    Circuit-ID ||
    Packet-Commitment ||
    Router-ID ||
    Router-Role ||
    Epoch ||
    Timestamp
)
```

## 5. Registration Classes

The reference policy vocabulary is:

| Class | Meaning |
|---|---|
| `PCC-0` | No registration |
| `PCC-1` | Origin registration |
| `PCC-2` | Country registration |
| `PCC-3` | Border-router registration |
| `PCC-4` | Developmental-region registration |
| `PCC-5` | Multi-country circuit |
| `PCC-6` | Full registered circuit |

Router roles are independently classified:

```text
ORIGIN
BORDER
TRANSIT
COUNTRY
REGIONAL
SOCIAL-SYSTEM
DESTINATION
SCIENCE
SCIENCE-HUB
```

The `SOCIAL-SYSTEM` role is an administrative registry category. It does not imply a political preference, a security grade, or a weaker/stronger cryptographic profile.

## 6. Country and Developmental-Region Registration

A country or developmental region may publish a PCC Router Registry containing authorized registration points. The registry should identify the router's public identity, role, validity period, applicable region, supported PCC profiles, and revocation state.

Example:

```text
Country: EXAMPLE
Region: DEVELOPMENTAL-REGION-01

PCC Registered Routers

BR-001    BORDER
RR-014    COUNTRY
RR-027    REGIONAL
SR-003    SOCIAL-SYSTEM
SC-011    SCIENCE
```

A country registry is a policy and trust object. It does not by itself authorize packet decryption.

## 7. Sender-Controlled Scope

A sender may request, subject to policy:

```text
REGISTER: Country only
REGISTER: Country + border
REGISTER: Developmental region + country
REGISTER: Full PCC circuit
REGISTER: Full circuit with individually verifiable receipts
```

The implementation should minimize registration metadata to the selected scope.

## 8. Registered Exchange

The **Registered Exchange** is the packet-to-router registration method. It is designed to fit into an HTTP/3/QUIC forwarding environment without requiring a router to terminate the application session.

A PCC-capable endpoint advertises support through an authenticated control-plane policy. The sender creates a circuit descriptor and requests one or more registration classes. The packet carries or is associated with a PCC commitment. A Registered Router validates the circuit policy, computes or verifies the commitment, records the observation, and returns a signed registration receipt through a PCC control exchange.

Conceptually:

```text
Sender
  |
  | HTTP/3 + QUIC packet
  | PCC commitment
  v
Envoy / HTTP gateway
  |
  | Registered Exchange metadata
  v
PCC-aware Registered Router
  |
  +-- validate circuit
  +-- validate router policy
  +-- record commitment
  +-- sign receipt
  |
  v
Receipt / acknowledgement
```

The Registered Exchange is control-plane metadata associated with the protected packet/flow. It must not require Envoy to expose HTTP/3 application plaintext merely to produce a registration receipt.

## 9. Envoy Integration Method

The preferred Envoy method is an extension/filter integration at a point where the proxy can observe connection, stream, route, and forwarding metadata without terminating encryption that it is not otherwise authorized to terminate.

The integration should provide:

1. PCC circuit lookup.
2. Registration-policy lookup.
3. Packet/flow commitment generation or verification.
4. Registered Exchange metadata creation.
5. Asynchronous receipt handling.
6. Receipt validation and bounded caching.
7. Registration failure policy.
8. Metrics and audit events without sensitive payload logging.

A conceptual Envoy path is:

```text
HTTP/3 / QUIC listener
        |
        v
PCC metadata / policy filter
        |
        +---- no PCC -> ordinary forwarding
        |
        +---- PCC -> commitment + Registered Exchange
                         |
                         v
                  Registered Router
                         |
                         v
                   signed receipt
                         |
                         v
                  continue forwarding
```

A registration service should be asynchronous where possible. PCC receipt failure MUST NOT automatically cause a plaintext fallback or cryptographic downgrade. If policy requires successful registration, the stream/flow should be rejected or held according to the configured fail-closed policy.

## 10. Relationship to Existing HTTP 3.0 Security

PCC is complementary to:

- Cryptographic Identity Capsules;
- Cryptographic Route Maps;
- route-session rekeying;
- Key Distribution Server bootstrap;
- jurisdictional capsule sets;
- HTTP/3 and QUIC encryption.

PCC does not replace any of them.

```text
                 HTTP 3.0
                    |
          +---------+---------+
          |                   |
       Route Map             PCC
          |                   |
    Route Session       Circuit Registry
          |                   |
       CIC Layer        Router Registry
          |                   |
          +---------+---------+
                    |
                 HTTP/3
                  QUIC
```

## 11. Privacy and Security Boundary

A PCC router may establish that a commitment was presented at its registration point. It should not thereby learn plaintext HTTP content.

PCC implementations MUST NOT introduce:

- universal interception keys;
- hidden decryption keys;
- plaintext copies of application traffic solely for registration;
- raw national identification numbers;
- raw telephone numbers;
- raw DNA or biometric identifiers.

A protected identity reference may be carried only when required by an independently authorized policy system.

## 12. Replay, Ordering, and Receipt Chains

Receipts should contain a circuit epoch and monotonically increasing registration sequence where practical. A deployment requiring strict ordering may chain each registration to the previous receipt commitment.

```text
Receipt N-1
    |
    v
commitment N-1
    |
    v
Receipt N
    |
    v
commitment N
```

A router must reject an invalid sequence transition according to the circuit policy. Replayed receipts must not be treated as new observations.

## 13. Failure Handling

PCC is a policy service and must have explicit failure behavior.

Possible policies are:

- `BEST_EFFORT`: continue forwarding if registration is temporarily unavailable;
- `RETRY`: hold the registration exchange for a bounded interval;
- `FAIL_CLOSED`: do not transmit when mandatory registration cannot be completed.

None of these policies may result in plaintext fallback or an automatic downgrade of HTTP 3.0 cryptographic protection.

## 14. Initial Implementation Profile

The initial PCC implementation should use modern authenticated cryptography already available in SLeeLa's OpenSSL integration. Router signatures should use an approved signature mechanism, and commitments should use an approved cryptographic hash with domain separation.

PCC should reuse the project's existing key lifecycle, trust-root, rotation, audit, and no-downgrade rules rather than inventing a separate trust hierarchy.

## 15. Status

PCC is an experimental HTTP 3.0 architecture component intended for continued implementation and testing. It should be treated as a registration/provenance protocol, not as a mechanism for universal surveillance or content interception.
