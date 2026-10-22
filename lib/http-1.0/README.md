# SLeeLa HTTP/1.0 library sources

This directory contains SLeeLa-language models and policy for the native
HTTP/1.0 implementation in the repository-root `/http-1.0` directory.
The C/C++ implementation remains responsible for sockets, parsing bytes,
serializing wire responses, and the server process.

## Source files

- `SLPackage.sleela` — standard-library package facade.
- `Http10Request.sleela` — request method, target, body metadata, and SLeeLa logical routing identifiers.
- `Http10Response.sleela` — status, content type, body, generation advertisement, and connection-close policy.
- `Http10Negotiation.sleela` — explicit generation selection with configurable HTTP/1.0 fallback.
- `Http10DownloadState.sleela` — resumable transfer metadata and the 50 MiB large-file-mode threshold.

## Boundary and safety

These classes describe application-level state; they do not open sockets or
claim that a request has been parsed or authorized. The native implementation
must validate wire input, enforce request/body limits, apply route and policy
checks, and serialize the final response. A logical SLeeLa port is distinct
from the operating-system TCP/UDP port.

The DOWNLOAD threshold selects resume-oriented behavior; it is not a maximum
file size. Transfer state carries SESSION-ID, DATETIME, FILE-ID, FILE-NAME,
INDEX, OFFSET, and TOTAL-SIZE metadata.

## Native implementation

See [`/http-1.0`](../../http-1.0/README.md) for the portable C/C++ server,
negotiation implementation, build targets, and native self-test.
