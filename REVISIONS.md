# REVISIONS

## 2026-09-22 — Server hardening

### Server Edition
- Added an atomic run lock to `server-edition/serverd` so overlapping server passes fail closed.
- Applied a private default umask to server-created state.
- Replaced the broad verification-output filter with explicit security/defender-line filtering.
- Preserved the one-shot, local-only execution model.

### HTTP 3.0 Key Distribution Server
- Made bootstrap-key file reads and writes resilient to short POSIX I/O operations.
- Reset the issuance window when the system clock moves backward instead of relying on unsigned timestamp subtraction.
- Added TTL overflow protection before computing contract expiration.
- Preserved TLS 1.3, X25519, HKDF-SHA-256, AES-256-GCM, request limits, connection limits, and issuance limits.

### Java HTTP Connector
- Added protocol-version and request-ID headers for request correlation.
- Added an explicit Accept header.
- Added configurable response-size limits and validation.
- Kept explicit request timeouts and structured failure results.

### Architecture
- Added `SERVER.md` documenting the distinction between local Server Edition and the network-facing HTTP 3.0 KDS.
- Documented the remaining portability boundary: the KDS listener still uses POSIX socket/thread APIs and needs a shared native platform adapter before Windows 10+ and macOS can be claimed as native server targets.

## Verification status

Source-level changes were committed to `main`. Native Windows/macOS server builds were not represented as verified by this revision.

## 2026-09-22 — Complete API documentation

- Added `/api/index.html` as the API documentation entry point.
- Added `/api/API.html` as a repository-wide source API catalog covering native C/C++, C++, Java, and SLeeLa source units.
- Included the Java connector class surfaces and existing Email Facade API documentation.
- Kept the catalog source-derived and avoided inventing undocumented signatures.

## 2026-09-22 — HTTP 3.0 extended logical ports

- Added a 160-bit HTTP 3.0 logical port type supporting exactly (10^{48}) distinct values, from 0 through (10^{48}-1).
- Added exact decimal conversion, validation, comparison, increment, and 20-byte network-order encoding in `http-3.0/http3_port.c`.
- Added the actual logical PORT field to HTTP 3.0 textual and binary envelopes.
- Included PORT in the canonical keyed-MAC input so a packet cannot silently change its destination/service port.
- Added envelope APIs for setting a decimal or uint64 port and resealing the packet digest.
- Added a boundary self-test and `http-3.0/PORTS.md`.
- Documented the distinction between the protocol's logical port namespace and smaller native operating-system socket-port limits.


## 2026-09-22 — README and build documentation refresh

- Updated `README.md` with the HTTP 3.0 extended logical-port model and the current protocol build commands.
- Updated `BUILD.md` with dedicated HTTP 3.0 verification targets for the logical-port boundary, native pipeline, and reference tests.
- Documented the requirement that future any-order authenticated packet fields be canonicalized before integrity verification rather than authenticated according to raw wire ordering.
## 2026-09-22 — Full HTML source coverage catalog

- Added `api/SOURCE-COVERAGE.html`, generated from the `master` Git tree, covering all recognized source-code files with language, file size, repository-area role, and direct source links.
- Updated `api/index.html` to expose the exhaustive source inventory.
- Updated `api/API.html` to reflect the current 1,090-file source inventory and link the exhaustive coverage document.
- Kept semantic API claims separate from structural source coverage so the documentation does not invent interfaces that are not present in source.

## 2026-09-22 — BODI XML projects

- Added the `/api/bodi` BODI XML API.
- Added a versioned XML schema for science and SMTP email projects.
- Added the native C++ `bodi-xml-runner` with bounded XML parsing and an allow-listed operation dispatcher.
- Added executable Math and Physics XML operations using the same formula families represented by SLeeLa native subject implementations.
- Added SMTP XML project support using the existing `api/email/sleela_email.h` facade.
- Kept SMTP submission dry-run by default; actual network submission requires `--send-email`.
- Kept SMTP credentials outside XML through `username-env` and `password-env` references.
- Added HTML API documentation and Math, Physics, and SMTP XML examples.
- Unknown disciplines and operations are rejected and witnessed rather than interpreted as arbitrary native or shell commands.


## 2026-09-22 — XML posts, listeners, and routers

- Added native C vocabulary for HTTP-style posts, listener declarations, and deterministic method/path routing.
- Added bounded validation and POST request serialization in `api/posting/sleela_post.c`.
- Added listener contract validation in `api/listener/sleela_listener.c`.
- Added exact route validation and matching in `api/router/sleela_router.c`.
- Extended the BODI XML runner with `post`, `listener`, and `router` project kinds.
- Extended the BODI XSD and added XML examples 07–09.
- Kept XML execution explicit: the new runner validates and witnesses these declarations without implicitly opening sockets or transmitting network traffic.


## 2026-09-22 — Apache/Tomcat Web Server Monitor

- Added `api/webserver/` as a cross-platform SLeeLa deployment and monitoring module for web-aware applications.
- Added a portable C configuration API with bounded server, port, health-path, and module-path validation.
- Added Linux and macOS shell adapters and a Windows 10+ PowerShell adapter.
- Added install, status, health, upgrade, port-change, validation, and explicit local Tomcat WAR deployment operations.
- Added backup-before-port-edit behavior on Unix adapters.
- Kept administrator/root operations explicit and prevented silent selection of unsupported Apache distributions on Windows.
- Added HTML API documentation and an example deployment environment profile.
