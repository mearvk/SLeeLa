# SLeeLa HTTP 5.0 library sources

This directory contains SLeeLa-language models that correspond to the native
reference implementation in `../../http-5.0/`. The generation name is a
SLeeLa project label, not an IETF HTTP/5 standard.

## Source inventory

- `SLPackage.sleela` — package facade and honest capability declarations.
- `Http50Frame.sleela` — 32-byte envelope fields, 13 frame types, flags and bounds.
- `Http50Request.sleela`, `Http50Response.sleela` — endpoint request and response models.
- `Http50Negotiation.sleela` — generation/capability negotiation policy.
- `Http50StreamState.sleela` — sequence, lifecycle and window policy model.
- `Http50DownloadState.sleela` — transfer metadata and commit preconditions.
- `Http50Limits.sleela` — explicit size, queue, stream and buffering defaults.
- `Http50FriendsPack.sleela` — Friends' Packs application data and quota model.

## Wire and implementation boundaries

The native frame header is 32 bytes, big-endian: version, type, flags, stream ID,
request ID, sequence, payload length, followed by the payload. The current native
codec enumerates OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME, CAPSULE,
FRIENDS_PACK, BONUS_OFFER, FP_UPDATE and AUDIT.

These SLeeLa files are policy/data models, not replacements for the native codec,
HTTP parser, transport, or enforcement. In particular, a model-level negotiation
flag is not proof of a cryptographically authenticated handshake; modeled stream
windows are not enforced flow control until native server dispatch applies them;
and DOWNLOAD fields are not a durable authenticated resume service.

The native server currently carries frames over HTTP/1.x on TCP. It does not
provide native HTTP/3/QUIC or TLS, and this package is not production-ready for
untrusted network exposure. Use a separately maintained authenticated transport,
perform authorization in native dispatch, and retain the repository SHA-256
integrity verification gate.

Native specifications and test vectors are in `../../http-5.0/`.
