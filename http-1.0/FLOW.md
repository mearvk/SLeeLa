# HTTP 1.0 Data Flow

## Request Path

```text
client
  ↓
native TCP endpoint
  ↓
HTTP/1.0 request
  ↓
SLeeLa envelope / route
  ↓
logical PORT
  ↓
SERVICE-ID
  ↓
OP-ID
  ↓
application handler
  ↓
HTTP/1.0 response
```

## Logical Port Multiplexing

Because HTTP 1.0 has no independently multiplexed HTTP streams, the logical-port layer is carried inside the SLeeLa application request.

A server may process multiple logical ports using one listening native transport endpoint. The implementation may serialize requests on a connection or use multiple HTTP/1.0 connections according to the surrounding HTTP implementation.

The important invariant is:

**logical PORT is an application route, not a native socket number.**

## Relationship to Later Versions

| Version | Transport multiplexing | SLeeLa logical port |
|---|---|---|
| HTTP 1.0 | Request/connection based | Application route |
| HTTP 2.0 / 2.1 | HTTP/2 streams | Application route per stream/request |
| HTTP 3.0 | QUIC/HTTP/3 streams | 160-bit application route |

This gives all three generations a common application-routing model while allowing each transport generation to retain its own native mechanics.


## Download Mode for Files > 50 MB

HTTP 1.0+ supports an application-level resumable download mode for files larger than 50 MB.

`SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`

A resumed request selects FILE-ID and its transfer INDEX/OFFSET rather than depending on the original connection remaining open.
