# SLeeLa HTTP 7.0 — Specification

**Status:** Experimental SLeeLa application-protocol generation; not an IETF HTTP/7 standard.

## 1. Purpose and semantic boundary

HTTP 7.0 extends the HTTP 6.0 32-byte big-endian frame envelope (20 frame types) with four Reality Assertion frame types. The protocol transports assertions as application data; transmission is not proof, authentication, or a truth guarantee.

## 2. Frame envelope

The 32-byte header fields, in network byte order, are: version (8 bits), type (8 bits), flags (16 bits), stream ID (64 bits), request ID (64 bits), sequence (64 bits), payload length (32 bits). Payloads are limited to 16 MiB. The current HTTP carrier limits request bodies to 8 MiB, so not every maximum-sized frame can be transported through the current HTTP endpoint.

Known types are 1–24. The defined low eight flag bits are FIN, ACK, URGENT, INCREMENTAL, RESUMABLE, OPTIONAL, USER_AUTHORED and FACTUAL; upper reserved flag bits must be zero. A frame decoder reports the consumed byte count; the HTTP H7 endpoint accepts exactly one complete frame per request.

## 3. Reality Assertion record

The assertion text payload uses exactly six tab-delimited fields:

    STATUS\tSTATEMENT\tSOURCE\tSOURCE_DATE\tAUTHOR\tDOCUMENT_REFERENCE\n

Fields cannot contain tabs, CR or LF. Statement is required. FACTUAL_DOCUMENTED requires a source; USER_AUTHORED requires an author. Unknown status names, malformed field counts, overlong fields and embedded NUL are rejected.

Statuses:
- FACTUAL_DOCUMENTED — presented as supported by identified documentary evidence; not independently certified by the transport.
- USER_AUTHORED — attributed to an author and not automatically verified.
- FICTIONAL — explicitly fictional.
- COUNTERFACTUAL — explicitly counterfactual.
- DISPUTED — disputed; no automatic resolution.
- UNVERIFIED — verification status is unknown or pending.

For an ASSERTION frame, FACTUAL and USER_AUTHORED flags must match the parsed status. ASSERTION_STATUS payloads must equal a known status token. Source/author metadata frames must be nonempty and contain no NUL or line breaks.

## 4. HTTP carrier

Built-in routes:
- GET /__sleela/health
- GET /__sleela/status
- GET /__sleela/negotiate
- POST /__sleela/h7

The current server uses one HTTP request per TCP connection, bounded body/header sizes, strict Content-Length parsing, no Transfer-Encoding, and a five-second socket read/write inactivity timeout. This is not an absolute request deadline. Binding defaults to loopback; explicit binding is required for other interfaces. Negotiation fallback is disabled unless --allow-fallback is passed.

## 5. Current limits and production requirements

This is a reference implementation, not a production-secure internet server. It does not yet implement TLS, peer identity/authentication, application authorization, total request deadlines, rate limiting, multiplexed stream lifecycle, flow control, replay protection, durable resumable transfer, or complete peer/request/stream attribution in the logger. The metadata-only shared logger bridge is wired, and its persisted-record test is part of the HTTP 7.0 test suite. Do not expose it directly to untrusted networks. See SECURITY.md.