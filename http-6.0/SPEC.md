# SLeeLa HTTP 6.0 — Core Specification

**Status:** Experimental SLeeLa application protocol. This is a project specification, not an IETF HTTP/6 standard. The reference TCP server is not production-certified and does not itself provide TLS, peer authentication, authorization, or a complete multiplexed session implementation.

## 1. Scope

HTTP 6.0 carries the HTTP 5.0 frame model and adds seven application-data frame types for a Consolidated Friends' Bet and Teamster Debate. The words “bet”, “points”, “IQ”, “team”, “recipient”, and “debate” denote user-authored application data; none is an identity assertion, permission, payment, routing command, endorsement, or guarantee of delivery.

The reference server uses HTTP/1.0 or HTTP/1.1 over the SLeeLa portable TCP socket layer. H6 binary frames are sent as the body of a POST /__sleela/h6 request. This is not HTTP/3 or QUIC.

## 2. Frame envelope

Every frame starts with a 32-byte network-byte-order header:

| Offset | Bytes | Field |
|---:|---:|---|
| 0 | 1 | protocol version (6) |
| 1 | 1 | frame type (1–20) |
| 2 | 2 | flags |
| 4 | 8 | stream ID |
| 12 | 8 | request ID |
| 20 | 8 | sequence |
| 28 | 4 | payload length |

The payload immediately follows the header. The protocol codec limits payloads to 16 MiB; the HTTP reference server limits request bodies to 8 MiB. Receivers validate version, type, reserved flag bits, payload bound, and available bytes before using payload data. The HTTP endpoint accepts exactly one frame per request; trailing bytes or a second frame are rejected.

### Frame types

| Code | Type | Meaning |
|---:|---|---|
| 1–13 | OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME, CAPSULE, FRIENDS_PACK, BONUS_OFFER, FP_UPDATE, AUDIT | HTTP 4/5 frame and control/application types |
| 14 | CONSOLIDATED_FRIENDS_BET | Combined friend records, points and document references |
| 15 | TEAMSTER_DEBATE | Structured debate package |
| 16 | CONSOLIDATE_IQ | User-supplied numeric application field |
| 17 | TEAM_AREA | User-authored team/area label |
| 18 | DEBATE_TOPIC | User-authored topic |
| 19 | DEBATE_POSITION | User-authored position/argument |
| 20 | RECIPIENT_LABEL | Optional audience/recipient label |

Flag bits 0–6 are FIN, ACK, URGENT, INCREMENTAL, RESUMABLE, OPTIONAL, and USER_AUTHORED. Bits 7–15 are reserved and must be zero.

## 3. Consolidated Friends' Bet data model

The C++ model serializes deterministic UTF-8 JSON using schema sleela-http6-consolidated-friends-bet-v1. The object includes a friends array (name, unsigned 32-bit points, document reference), an unsigned 64-bit consolidate_iq, team_area, debate_topic, debate_position, and recipient_label.

Field bounds are byte limits, not character counts:

- friends: at most 256 records
- friend name: 128 bytes, non-empty
- document reference: 512 bytes
- team/area: 128 bytes
- debate topic: 1,024 bytes
- debate position: 8,192 bytes
- recipient label: 256 bytes

Strings must be valid UTF-8. JSON control characters, quotes and backslashes are escaped. A document reference is data only; dereferencing it requires separate application authorization and path/URL policy.

## 4. HTTP surface

- GET /__sleela/health — simple liveness response.
- GET /__sleela/status — JSON implementation/frame inventory.
- GET /__sleela/negotiate — version negotiation based on X-SLeeLa-Version and X-SLeeLa-Accept.
- POST /__sleela/h6 — exactly one binary H6 frame in the request body.

Request body length is bounded at 8 MiB. The server binds to 127.0.0.1 by default; a wildcard or external interface must be selected explicitly with --bind. The reference implementation accepts decimal Content-Length only; it rejects duplicate Content-Length, Transfer-Encoding, malformed lengths, and incomplete bodies. It closes each connection after one response. It does not implement chunked transfer coding or keep-alive.

Negotiation is capability selection, not authentication. Lower-version fallback must be explicitly enabled by server policy and must not be used to bypass security requirements. Preferred-router and resolver policies are project hooks, not substitutes for transport security or authorization.

## 5. Security requirements

1. Deploy behind a properly configured TLS-terminating endpoint or add and validate a native TLS layer before exposing it to untrusted networks. Do not send sensitive application data over unauthenticated cleartext TCP.
2. Treat all headers, frame fields, labels, document references, and payloads as untrusted input.
3. Reject malformed HTTP framing, overlong fields, duplicate framing headers, unsupported transfer coding, invalid frame headers, reserved flag bits, truncated bodies, and trailing frame bytes.
4. Apply request deadlines, connection limits, rate limits, and bounded concurrent work at the deployment boundary. The current single-connection reference loop is not a hardened high-load service.
5. A recipient label does not prove recipient identity. A points balance does not authorize an operation. A document reference does not grant file access.
6. Log security-relevant failures without logging sensitive payload contents by default.
7. Never describe a successful parse or empty result as proof that content or peer is safe.

## 6. Conformance and release gates

make syntax, make test, and make pack-test must pass on Linux x86-64, Windows x86-64, macOS x86-64, and macOS arm64 for the exact commit. Tests cover malformed requests, strict body framing, frame round trips, reserved bits, JSON escaping, UTF-8 validation, field bounds, and friend-count limits. Passing these tests does not replace fuzzing, interoperability testing, TLS/authentication review, or an independent security assessment.
