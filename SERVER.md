# SLeeLa Server Architecture

## Purpose

SLeeLa has two server surfaces:

1. **Server Edition** — a local, file-backed processing runtime. It intentionally does not expose a public listener.
2. **HTTP 3.0 Key Distribution Server (KDS)** — the network-facing TLS bootstrap service for HTTP 3.0 key contracts.

These surfaces have different trust boundaries and must not be treated as interchangeable.

## Server Edition

`server-edition/serverd` is a one-shot supervisor. It:

- resolves the SLeeLa executable;
- requires the repository SHA-256 execution gate when configured;
- provisions the private inbox/state directories;
- prevents overlapping runs with an atomic state-directory lock;
- runs from the repository root so relative server paths are deterministic;
- appends a timestamped execution record to `server-edition/state/results.log`.

The server language program remains local and quiet. It processes the batch supplied in its inbox rather than opening a socket.

## HTTP 3.0 KDS

The KDS listener provides:

- TLS 1.3 minimum;
- X25519 server bootstrap identity;
- encrypted bootstrap contracts using X25519 + HKDF-SHA-256 + AES-256-GCM;
- bounded request/body/response sizes;
- concurrent connection limits;
- issuance rate limiting;
- strict client identifier and jurisdiction validation;
- fail-closed certificate/private-key validation.

The long-lived X25519 bootstrap key is stored separately from the TLS certificate/key pair.

### Operational requirements

Do not expose the KDS directly to the public Internet without an explicit deployment policy, firewalling, certificate lifecycle, and monitoring plan.

The current native listener uses POSIX socket/thread APIs. The cryptographic contract layer is portable C, but the listener itself still requires a platform adapter before it can be described as native Windows 10+ or macOS server support.

## Java Connector

`SleelaHttpConnector` now sends:

- `X-SLeeLa-Protocol-Version: 1`
- a unique `X-SLeeLa-Request-ID`
- an explicit `Accept` header

Responses are bounded before conversion to text, and the maximum response size is configurable through the constructor. Timeouts remain explicit rather than relying on the Java runtime default.

## Next server hardening boundary

The next native-network change should introduce one socket/thread abstraction shared by Linux, Windows 10+, and macOS. The KDS request/response and cryptographic contract code should remain above that abstraction. This avoids duplicating security logic while removing the current POSIX-only listener dependency.
