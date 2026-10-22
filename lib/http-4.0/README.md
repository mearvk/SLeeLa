# SLeeLa HTTP 4.0 — application protocol models

HTTP 4.0 is the next SLeeLa application-protocol generation derived from the
HTTP 3.0 model inventory. These .sleela files define library-level data models
and policy decisions; they do not prove that native transport, cryptography,
flow control, or wire behavior has been implemented.

## Model inventory

- SLPackage.sleela — package facade and capability declaration.
- Http40Envelope.sleela — v4 envelope, logical port, session/stream identity,
  sequence, payload length and explicit digest-algorithm label.
- Http40Request.sleela and Http40Response.sleela — request and correlated response.
- Http40Negotiation.sleela — authenticated generation/capability negotiation.
- Http40Integrity.sleela — keyed digest, nonce, INTACTX, BASKET and authorization gate.
- Http40StreamState.sleela — independent stream sequencing, half-close and reset.
- Http40DownloadState.sleela — resumable transfer metadata.
- Http40Limits.sleela — payload, header, stream, queue and dictionary policy defaults.

## Preserved HTTP 3.0 baseline

Preserves the 160-bit logical-port namespace (0 through 10^48−1), request
correlation, keyed-MAC DIGEST model, nonce replay guard, INTACTX check, canonical
14-item / 172-byte BASKET metadata, L1–L4 capabilities, and DOWNLOAD fields:
SESSION-ID, DATETIME, FILE-ID, FILE-NAME, INDEX, OFFSET, TOTAL-SIZE.
Large-file mode begins above 52,428,800 bytes; this is not a file-size cap.

## HTTP 4.0 model additions

1. Session-scoped stream identity and independent send/receive sequence counters.
2. Stream close/reset state and bounded concurrent-stream policy.
3. Authorization as a distinct dispatch-gate requirement.
4. Explicit digest-algorithm label for future negotiation.
5. Header and queue limits in addition to HTTP 3.0 payload/dictionary bounds.
6. Explicit authenticated-offer and legacy-fallback decisions.

## Security and implementation boundaries

The SLeeLa generation label is not the IETF HTTP/3 version. Do not claim QUIC,
HTTP/3 framing, QPACK, transport encryption, algorithm agility, persistent replay
protection, or enforcement of these limits until native implementation and tests
demonstrate them. HTTP 3.0's reference documentation describes an HTTP/1.x carrier.
Production deployment still requires authenticated TLS/QUIC or another protected
transport. SipHash-2-4's 64-bit tag is not a replacement for modern transport
security or peer identity authentication.

Before calling HTTP 4.0 production-ready, port native behavior, update the wire
specification, test malformed input/replay/stream resume, compare C/C++ behavior,
run sanitizers and fuzzing, and verify manifests and CI. Model counters alone are
not a security boundary.
