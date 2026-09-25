# HTTP 4.0 Source — Initial Status

As of: 2026-09-25  
Status: Experimental foundation.

## Important standards note

There is no IETF HTTP/4 standard to implement at this time. The IETF HTTP working group remains active on HTTP semantics and extensions. This repository therefore calls this implementation SLeeLa HTTP 4.0, meaning a proposed next-generation SLeeLa protocol generation rather than an official HTTP version.

## Implemented

- fixed-width HTTP 4.0 frame header;
- bounded payload validation;
- explicit stream/request/sequence identifiers;
- OPEN/DATA/END/RESET/WINDOW/PING/PONG/RESUME/CAPSULE frame taxonomy;
- incremental and resumable flags;
- deterministic encode/decode;
- C11 self-test;
- capability model;
- flow-control, resumability, migration, and carrier-boundary documentation.

## Not yet implemented

- HTTP/3/QUIC carrier;
- native network socket transport;
- cryptographic session authentication;
- persistent resume store;
- complete stream state machine;
- multi-path scheduling;
- congestion-control integration;
- browser interoperability;
- formal conformance corpus;
- Windows/macOS/Linux CI for the HTTP 4.0 directory.

## Relationship to HTTP 3.0

HTTP 3.0 remains the existing SLeeLa application protocol. HTTP 4.0 does not overwrite it. The new generation is additive and can initially be carried through HTTP/3 infrastructure.

## Immediate next work

1. Build a complete session state machine.
2. Add C/Python parity.
3. Add an HTTP/3 carrier adapter.
4. Add resumable transfer state.
5. Add authenticated sequence protection.
6. Add conformance fixtures.
7. Add platform build scripts and CI.


## Packet context extension — 2026-09-25

Added an extended packet-context layer containing a grade-12-or-higher
education benchmark token and the canonical HTTP/3 basket. The context is
wrapped before application content by the new packet-payload helper.

Added an optional curl/wget freshness loop with eight GET attempts per minute
(one every 7.5 seconds). The loop is intentionally outside the frame transport
path and never uploads packet contents.
