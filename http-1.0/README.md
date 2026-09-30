<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">






# SLeeLa HTTP 1.0

**Status:** Experimental SLeeLa application-protocol generation.

SLeeLa HTTP 1.0 establishes the baseline application model used by the later HTTP generations in this repository. It keeps transport addressing separate from SLeeLa's logical service-routing identifiers.

## Architecture

```text
Native transport endpoint
        |
HTTP/1.0 request / connection
        |
SLeeLa logical PORT
        |
SERVICE-ID / OP-ID
        |
Application operation
```

A SLeeLa **logical port** is an application-level identifier. It is not a replacement for, or necessarily a one-to-one mapping with, a native TCP or UDP port.

## Multiplexing

HTTP/1.0 does not provide HTTP/2-style stream multiplexing. SLeeLa therefore treats concurrent application exchanges as logical operations above the HTTP/1.0 request and connection boundary.

## Compatibility

The HTTP 1.0 layer is designed to coexist with conventional HTTP infrastructure. TCP sockets, TLS termination, proxies, routers, and native port bindings remain transport concerns; SLeeLa logical ports remain application-routing concerns.

## Large-File Download Mode

SLeeLa HTTP 1.0 and later generations define a common **DOWNLOAD** mode for files larger than 50 MB.

Resume metadata:

```text
SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE
```

- **FILE-ID** identifies the transfer.
- **INDEX** identifies a segment or chunk.
- **OFFSET** identifies the byte position.
- **TOTAL-SIZE** records the complete transfer size.

The 50 MB threshold selects the resume-oriented mode; it is not a maximum file size.

## Scope

HTTP 1.0 provides the foundational routing and transfer model. Later SLeeLa generations add additional framing, multiplexing, integrity, capability, and application features without changing the distinction between transport and application addressing.