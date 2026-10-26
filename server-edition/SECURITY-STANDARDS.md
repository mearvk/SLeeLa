# SLeeLa Server Edition — Security and Standards Baseline

**Document date:** 2026-10-26  
**Scope:** server-edition/  
**Status:** Initial hardening baseline with mutual-TLS transport and identity-operation authorization implemented in source; compilation, integration testing, and production review remain outstanding.

## Security posture

The server should be treated as a transport/admission component, not as a production-secure application gateway, until authentication, protected transport, authorization, bounded concurrency, deterministic shutdown, and service dispatch are implemented and tested.

Current hardening changes set the default listener to 127.0.0.1:19866, validate CLI port and packet-size values, reject unknown CLI options, and require an explicit --allow-remote flag before the command-line executable will attempt a non-loopback bind. The remote-bind flag alone is insufficient: remote use additionally requires TLS 1.3, mutual client-certificate verification, an allowlisted certificate identity, and explicit service/operation permissions. These source changes have not yet been build- or runtime-verified.

## Required engineering controls

### Network and identity

- Default to loopback; expose remotely only behind a documented, authenticated, protected deployment boundary.
- **Implemented in source:** TLS 1.3-only transport, mandatory client-certificate validation against a configured CA, explicit certificate-CN allowlisting, and exact identity/service/operation authorization rules. Remote binds require all these settings.
- **Still required:** Build and integration verification, certificate expiry/revocation and rotation procedures, and cross-platform OpenSSL packaging.
- **Forward secrecy:** TLS 1.3 configuration explicitly restricts supported ephemeral key-exchange groups to X25519, P-256, and P-384. The integration suite now attempts a TLS 1.3 handshake with X25519 as the only offered group; its result remains pending CI execution on each supported platform.\n- **Router/NAT awareness:** Added an opt-in STUN binding probe that reports local and externally mapped addresses without changing router/firewall state. It is diagnostic only and cannot prove inbound reachability or distinguish every NAT topology.\n- **Automated checks:** Added C++ protocol tests, Python STUN parser tests, a loopback mutual-TLS integration test (trusted client, unauthorized operation, missing client certificate, untrusted client CA), and a GitHub Actions build/test workflow targeting Linux, macOS, and Windows. These tests have not yet been observed passing; workflow results must be checked before claiming any platform passes.\n- Treat packet metadata, claimed source fields, grades, classifications, and annotation fields as untrusted input, never as identity or privilege.
- Add replay protection and durable request/sequence semantics for operations with side effects.

### Input and resource safety

- Validate every numeric field before narrowing casts; reject invalid values rather than wrapping them.
- Bound header, payload, complete-frame, active-connection, worker, and logging resources independently.
- Use absolute monotonic deadlines for complete reads; handle partial TCP writes and interruptions.
- Apply global and per-peer rate limits with a documented burst/window policy.
- Ensure parser failures cannot leave partially trusted packets eligible for dispatch.
- Keep malformed-input responses generic enough to avoid exposing internals while preserving useful audit records.

### Lifecycle and concurrency

- Use managed workers, a worker pool, or another joinable execution model.
- Synchronize lifecycle state and active-socket ownership.
- Stop accepting new connections, close or interrupt active work safely, join workers, then destroy loggers and shared state.
- Pair platform initialization and cleanup on every code path.
- Test repeated start/stop, shutdown under load, and resource-exhaustion cases.

### Logging and privacy

- Emit structured, escaped records with stable event names and UTC timestamps.
- Never log credentials, private keys, session tokens, or full payloads by default.
- Ensure audit records distinguish attempted, accepted, rejected, and successfully transmitted events.
- Define log rotation, permissions, retention, and disk-exhaustion behavior.
- Test concurrent logging and failure paths.

### Build and release

- Enable strict compiler warnings and add sanitizer builds where supported.
- Maintain automated builds/tests for each platform claimed as supported.
- Track third-party dependencies and update them through reviewed changes.
- Use reproducible build/test instructions and retain test evidence by commit.
- Do not claim a protocol generation or adapter is supported until conformance tests exercise the actual implementation.

## Standards and guidance to map against

These are engineering references, not claims of certification or compliance.

- **RFC 9293 — Transmission Control Protocol (TCP):** TCP is a byte stream; code must handle partial reads and writes and must not assume message boundaries.
- **RFC 8446 — TLS 1.3:** Reference for a protected transport if TLS is implemented. Certificate and peer-identity validation remain essential.
- **RFC 9110 — HTTP Semantics:** Apply only if/when the software actually implements HTTP semantics. SLPK framing and SLeeLa generation metadata alone are not native HTTP.
- **CWE Top 25 / CWE Catalog:** Use for weakness tracking, especially improper input validation, resource exhaustion, race conditions, and use-after-free risks.
- **OWASP ASVS:** Use as a verification checklist for authentication, authorization, validation, logging, and secure configuration; applicability depends on the deployed application.
- **NIST SP 800-218 (Secure Software Development Framework):** Use for change review, security testing, vulnerability response, and release evidence.

## Minimum release gate

A release must not be marked production-ready until all are true:

1. Build and automated tests pass on every advertised platform.
2. Transport tests cover truncated/oversized frames, partial I/O, timeouts, and invalid numeric fields.
3. Concurrency tests cover connection floods, active-connection limits, races, and shutdown under load.
4. Security tests verify authenticated transport, identity validation, per-service authorization, and replay behavior.
5. End-to-end tests prove that authorized packets reach registered service handlers and unauthorized packets do not.
6. Documentation matches tested behavior and clearly states any deployment-provided controls.
7. A reviewer records the commit, test commands, results, known limitations, and release decision.

## Related documents

- [1-2-3-4 remediation plan](1-2-3-4.md)
- [Production guidance](PRODUCTION.md)
- [Wire protocol](WIRE-PROTOCOL.md)
- [Port-awareness contract](PORT-AWARENESS.md)
- [HTTP generation notes](HTTP-GENERATIONS.md)
