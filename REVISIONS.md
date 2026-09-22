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
