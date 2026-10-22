# HTTP 4.0 Production Gates and Implementation Plan

This document tracks work required before SLeeLa HTTP 4.0 can be described as production-ready. It intentionally distinguishes a reusable policy primitive from an integrated, tested production feature.

## Gate 0 — Repository integrity and reproducible builds

- [ ] Review the five mismatches reported by the native portability workflow on commit `7a1b867148b039b66b50ae1604e2e005d33da6bf`.
- [ ] Confirm each file's intended source history before changing `security/sha256-manifest.json`.
- [ ] Keep the manifest verification mandatory; do not disable or skip it to make CI green.
- [ ] Rerun the full native workflow on Linux, macOS, and Windows/MinGW-w64 after approved corrections.

## Gate 1 — Complete frame payload schemas

Define and enforce a versioned payload schema for OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME, and CAPSULE. Each schema must state:

- permitted flags and whether FIN/ACK are meaningful;
- stream-ID and request-ID constraints;
- exact or bounded payload length and byte order;
- legal session states and whether the frame is idempotent;
- malformed, unknown-extension, and future-version behavior.

Tests must prove that malformed control payloads are rejected before state mutation. Unknown control payload extensions must not be silently interpreted as known semantics.

## Gate 2 — Persistent stream dispatch and flow control

The current reference server accepts one frame in each HTTP request. A production stream engine requires a persistent session/dispatch abstraction rather than attaching the current state model to a one-frame handler without lifecycle guarantees.

Required behavior:

- Bind a validated frame to an authenticated session and a unique stream.
- Enforce per-direction sequence numbers and reject duplicate, skipped, or exhausted sequences according to the negotiated profile.
- Maintain receive credit and send credit independently. DATA must never exceed the peer's advertised credit; WINDOW increments must be nonzero and must not overflow negotiated bounds.
- Define connection-wide and per-stream byte, frame, queue, and active-stream limits.
- Specify RESET and END terminality, cleanup, cancellation, and timeout semantics.
- Ensure rejected frames do not partially mutate state; add allocation-failure and state-machine property tests.
- Integrate the model into a persistent carrier path and exercise concurrent streams and backpressure in CI.

## Gate 3 — Authenticated carrier boundary

The current code is HTTP/1.x over TCP; it is not a QUIC implementation. Do not hand-roll QUIC or TLS in the frame parser.

- Select and pin a maintained HTTP/3/QUIC implementation with a documented C API and supported platforms.
- Add a carrier adapter that reports authenticated peer identity, negotiated ALPN/profile, handshake completion, replay/0-RTT status, and stream lifecycle.
- Require authentication before processing state-changing operations; disable 0-RTT for non-idempotent operations unless application replay protection is proven.
- Define an explicit authenticated-TLS boundary for non-QUIC TCP deployments.
- Add interoperability tests and certificate/identity failure tests before advertising HTTP/3 support.

Until this gate is complete, deploy only behind an independently authenticated transport and keep the native server loopback-only by default.

## Gate 4 — Durable, authenticated resume

The current resume module provides policy checks and delegates verification to callbacks. Production requires an actual trusted implementation:

- Use vetted cryptographic primitives and a defined token format; bind tokens to principal, transfer ID, file version, total size, digest algorithm, committed offset, and expiry.
- Authenticate every chunk and enforce exact offset progression, bounded chunk sizes, and anti-replay semantics.
- Persist checkpoints transactionally and make checkpoint advancement atomic with durable data writes.
- On restart, validate checkpoint integrity and reconcile partially written chunks.
- Publish completed files atomically only after recomputing and matching the final digest.
- Test process termination at every persistence boundary, expired/forged tokens, stale state, disk-full, and concurrent resume attempts.

No default or placeholder verifier may return success in production configuration.

## Gate 5 — Security validation and release evidence

- Run sustained coverage-guided fuzzing against frame, segment, context, DOWNLOAD, HTTP request, and resume parsers.
- Retain sanitizer logs, reproducible test commands, cross-platform results, and corpus artifacts.
- Add C and SLeeLa shared fixtures for valid and invalid frames and transfer-state transitions.
- Complete threat modeling, resource-exhaustion review, dependency review, and independent security review.
- Keep the capability matrix truthful: mark a feature implemented only when the runtime path and tests exist, not merely when a type or policy API exists.

## Current disposition

HTTP 4.0 remains **experimental / not production-ready**. The HTTP 4.0 conformance workflow passed on commit `7a1b867148b039b66b50ae1604e2e005d33da6bf`, but the repository-wide native portability workflow failed at SHA-256 verification on five files before compilation. See [STATUS.md](STATUS.md) and [native workflow run](https://github.com/mearvk/SLeeLa/actions/runs/39711902376).
