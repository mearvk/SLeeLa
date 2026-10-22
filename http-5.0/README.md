<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="Debian logo">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa logo">

# SLeeLa HTTP 5.0

**Status: Experimental SLeeLa application protocol; not production-ready and not an IETF HTTP/5 standard.**

**Platforms targeted:** Linux · macOS · Windows 10+

HTTP 5.0 is a SLeeLa-specific application-protocol generation. The current reference server carries HTTP 5.0 frames inside HTTP/1.x over TCP. It does **not** implement HTTP/3 or QUIC and does not provide TLS or peer authentication.

## Build and test

```sh
make clean all
make syntax
make test
make server-test
make proto
make clean
```

The commands above describe the intended project targets; the latest source changes still require execution in CI before being called verified.

## Architecture

```text
SLeeLa application
    |
HTTP 5.0 endpoint and frame envelope
    |
Friends' Packs application data (optional extension)
    |
HTTP/1.x request handling
    |
SLeeLa portable TCP socket layer
```

## Frame model

The frame header is 32 bytes, big-endian:

```text
VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD
  1B      1B     2B       8B          8B           8B          4B             N bytes
```

The implementation enumerates 13 frame types: OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME, CAPSULE, FRIENDS_PACK, BONUS_OFFER, FP_UPDATE, and AUDIT. See [SPEC.md](SPEC.md) for the current wire contract and known gaps.

## Friends' Packs

Friends' Packs are ordinary application data for user-managed friend entries, points, document references, and optional bonus-offer references. FP is an application quota only; it is not an identity, trust score, authentication token, or authorization grant. Bonus references do not grant access on their own.

## DOWNLOAD metadata

The current metadata contract for transfers above the 50 MiB threshold is:

`SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`

These values are not authorization credentials. Durable resumability requires authenticated tokens, persistent atomic checkpoints, chunk verification, full-object digest validation, and crash-safe publication beyond the metadata parser.

## Security

Do not expose the current server directly to an untrusted network. Use an independently maintained authenticated transport and enforce application authorization. The current implementation has no native TLS/QUIC adapter, complete stream-state machine, or durable resume service.

## Current status

- [Core specification](SPEC.md)
- [Canonical envelope test vectors](TEST-VECTORS.md)
- [Security boundary](SECURITY.md)
- [Implementation status and release gates](STATUS.md)
- [Processing flow](FLOW.md)
- [Negotiation notes](HTTP.NEGOTIATION.md)

The repository's SHA-256 integrity gate must remain enabled. Review changed source and regenerate approved manifest entries only through the project's established verification process.
