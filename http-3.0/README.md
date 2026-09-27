# SLeeLa HTTP 3.0

Status: SLeeLa application/protocol generation; not a claim that SLeeLa-specific behavior is the IETF HTTP/3 standard.

HTTP 3.0 combines a compact application envelope, fast service/operation naming, request correlation, retry classes, processing pipeline, and an integrity/security substrate. It can use HTTP/3/QUIC as a carrier while keeping SLeeLa logical routing separate from native transport endpoints.

## Multiplexing

`QUIC connection → HTTP/3 stream → SLeeLa logical PORT → SERVICE-ID / OP-ID → REQUEST-ID`

Logical ports are application identifiers, not native sockets.

## Download mode

Files larger than 50 MB use the common SLeeLa DOWNLOAD mode:

`SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`

## Security boundary

The directory contains the SLeeLa application integrity substrate and its self-tests. Carrier security and application integrity remain distinct layers.

## Build and verification

See `STATUS.md`, `FLOW.md`, and the root `Makefile`. The build directory delegates to the protocol tree so the source implementation remains authoritative.
