<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">


# SLeeLa HTTP 2.0 / 2.1

**Status:** Experimental SLeeLa application-protocol generation; HTTP/2 compatibility is a transport concern.

HTTP 2.0 / 2.1 extends the SLeeLa HTTP 1.0 model with compact application envelopes, service and operation identifiers, request correlation, retry classes, and logical-port routing over concurrent HTTP/2 exchanges.

## Architecture

```text
Native transport endpoint
        |
HTTP/2 stream
        |
SLeeLa logical PORT
        |
SERVICE-ID / OP-ID
        |
REQUEST-ID
        |
Application request
```

Logical ports are application identifiers and are independent of native TCP/UDP socket numbering.

## Core Capabilities

- Compact application envelopes.
- Service and operation identifiers.
- Request correlation.
- Retry classification.
- Logical-port routing.
- Concurrent stream-aware exchanges.
- Compatibility with the repository's shared download/resume contract.

## Large-File Download Mode

Files larger than 50 MB use SLeeLa **DOWNLOAD** mode:

```text
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

This metadata allows an interrupted transfer to be identified and resumed.

## Negotiation

Generation selection is explicit. A peer must accept the proposed generation before it is selected. Where fallback is permitted, the negotiation layer may return to HTTP/1.1 and then HTTP/1.0.

Fallback must not silently remove a security property required by the application or deployment policy.

See `HTTP.NEGOTIATION.md` for the repository negotiation model.

## Build

```sh
make -C http-2.0
make -C http-2.0/build syntax
```

The 2.1 implementation in this directory is treated as the current core/sketch for this generation.