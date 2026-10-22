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

## Validation, authorization, and deployment policy

The SLeeLa policy models are declarative counterparts to the native C/C++ HTTP
transport. Native parsing is authoritative for wire-level syntax and bounds.
Application routing must validate logical port/service/operation identifiers
and authorize each protected operation; those identifiers are not credentials.
Download resume state must be tied to an authenticated principal and stable file
identity, with offset bounds checked and the final artifact digest verified.

Plain HTTP provides no confidentiality. TLS must be implemented in the native
transport or terminated by a trusted proxy. Forwarded identity headers are
trusted only when the connection is known to originate from that configured
proxy. The native server enforces 10-second total header/body read deadlines and
closes each connection after one response; it is currently single-connection
and does not provide native TLS or built-in application authentication.

The native regression and fuzzing entry points live under `tests/http-1.0`.
Use `make -C http-1.0 syntax test`, `make -C http-1.0 sanitize`, and (with
Clang/libFuzzer) `make -C http-1.0 fuzz-download`. Keep the generated SHA-256
manifest synchronized with changes to trusted `.sleela` library files by
running `python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json`.
