# HTTP 4.0 Resumability

Resumability is a protocol feature rather than an assumption about a particular carrier.

A transfer that is marked RESUMABLE has:
- a stable application transfer identifier;
- a logical stream ID;
- a request ID;
- a monotonically increasing frame sequence;
- an application-defined byte offset.

A RESUME exchange confirms the last sequence and application offset accepted by the receiver.

## Safety rule

A receiver MUST NOT silently skip an unknown range. If the requested continuation point cannot be proven compatible with stored state, the receiver should reject the resume attempt and require a new transfer.

## Large transfers

HTTP 4.0 is intended to support long-running transfers without requiring the original network path or carrier connection to remain alive. Persistent transfer state belongs in a separate storage subsystem.

This is intentionally compatible with the direction of current HTTP work on resumable uploads, but the SLeeLa mechanism is independent and experimental.


## Native policy gate

The C reference now includes `http4_resume.h/.c`. It checks that resume requests match the saved principal, file version, total size, exact committed offset, and expiry; chunk acceptance requires the next contiguous offset and a verifier callback; commit requires the full byte range and a final-digest verifier callback.

The callbacks are trust boundaries. Production must provide cryptographic token/chunk/final-digest verification and persist state atomically. The self-test uses deterministic fake callbacks to test policy ordering only; it does not prove cryptographic security or crash-safe persistence.
