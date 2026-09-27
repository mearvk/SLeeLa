# SLeeLa HTTP 2.0 / 2.1

Status: SLeeLa application-protocol generation; HTTP/2 compatibility is a transport concern.

HTTP 2.0/2.1 provides compact application envelopes, service/operation identifiers, request correlation, retry classes, and logical-port routing over HTTP/2 streams. The logical-port namespace is independent of native TCP/UDP sockets.

The 2.1 source is the current sketch/core in this directory; it deliberately precedes the additional integrity substrate introduced by HTTP 3.0.

## Multiplexing

`native transport endpoint → HTTP/2 stream → SLeeLa logical PORT → service/operation → request`

## Download mode

Files larger than 50 MB use SLeeLa DOWNLOAD mode with resume metadata:

`SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`

## Negotiation

Use `HTTP.NEGOTIATION.md` and the negotiation C/C++ implementation. A SLeeLa generation is selected only after explicit peer acceptance; permitted fallback is HTTP/1.1, then HTTP/1.0. Fallback must not silently weaken required security.

## Build

`make -C http-2.0` or `make -C http-2.0/build syntax`.
