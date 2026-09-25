# SLeeLa HTTP Architecture

## Generations

SLeeLa contains HTTP 1.x, HTTP 2.0/2.1, HTTP 3.0 and experimental HTTP 4.0 work.

## HTTP 4.0 status

HTTP 4.0 is an experimental SLeeLa protocol generation, not an IETF HTTP/4 standard. HTTP/3 is standardized as RFC 9114 and relies on QUIC; QUIC incorporates TLS 1.3 at the transport layer. citeturn0search0turn0search2

## HTTP 4.0 model

SESSION → STREAM → FRAME → SEQUENCE → DATA → END/RESET

Frame types include OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME and CAPSULE.

## Segmentation

Large logical application payloads may be divided into MTU-aware DATA frames using an explicit segmentation header. Reassembly validates stream/request identity, segment identity, indexes, counts, offsets, lengths, duplicates and overlap before exposing the completed payload.

## Transport security

TLS is not duplicated in the HTTP 4.0 frame encoder. HTTP/3/QUIC provides its own TLS 1.3 transport security. Other network carriers must expose an authenticated/integrity-protected transport contract before production use.

## Future work

Carrier negotiation, authenticated resumption, replay policy, timeout/eviction policy and message-level integrity can be extended without changing the basic frame model.

**Max Rupplin — MEARVK LLC — 2026**
