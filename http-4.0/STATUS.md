# HTTP 4.0 Source — Current Engineering Status

**Status:** Experimental foundation; not production-ready.  
**Protocol name:** SLeeLa HTTP 4.0, not an IETF HTTP/4 standard.  
**Carrier currently implemented:** HTTP/1.x over TCP in the reference server. No HTTP/3/QUIC adapter is present.

## Present in source

- 32-byte big-endian frame header and nine frame types.
- Bounded frame payload validation and deterministic encode/decode.
- Segmentation and out-of-order reassembly primitives.
- Packet context and basket metadata.
- HTTP server endpoint and built-in self-tests.
- Strict DOWNLOAD metadata parsing now rejects malformed unsigned integers, extra fields, unsafe path-like filenames, unterminated fixed-size strings, empty fields, and offset greater than total size.
- HTTP request framing now rejects duplicate Content-Length, Transfer-Encoding (unsupported by this parser), malformed/oversized Content-Length, undeclared request bodies, body bytes beyond the declared length, and truncated bodies; raw header buffers are explicitly NUL-terminated before string parsing.
- HTTP body limit is aligned with one maximum-size native frame (header plus 16 MiB payload).
- Frame decoder now rejects undefined flag bits.
- New bounded per-stream reference state model enforces per-stream sequence, stream byte, aggregate byte, duplicate OPEN, and monotonic/non-reused stream-ID rules.
- New resume-policy gate validates principal/file-version/size/offset/expiry bindings and requires chunk/final-digest verifier callbacks; this is policy enforcement, not a cryptographic verifier or durable store.
- New normative draft specification, conformance plan, capability matrix, and optional libFuzzer harness.

## Not yet established

- The repository changes in this status update have **not been compiled or executed in this interaction**. Do not infer that the tests pass.
- The new stream-state model is not yet wired into the server's frame-dispatch path.
- WINDOW credit enforcement and a complete bidirectional stream/session state machine are not integrated.
- The server still uses HTTP/1.x over TCP and does not provide TLS/QUIC itself.
- No authenticated HTTP/3/QUIC carrier adapter.
- No durable authenticated resume store, production token/chunk/final-digest verifier, or crash-safe final publication; the resume policy module relies on externally supplied verification callbacks.
- No native application-frame MAC/key-rotation implementation.
- Capability negotiation is not yet bound to an authenticated transcript.
- No completed C/SLeeLa shared conformance vectors.
- No recorded cross-platform CI results, sanitizer results, fuzz corpus, or independent security review.

## Documents

- SPEC.md — normative frame and security rules.
- CONFORMANCE.md — required deterministic/adversarial tests.
- CAPABILITY-MATRIX.md — implementation evidence and release gates.
- SECURITY.md — carrier boundary and security requirements.
- RESUME.md — resumability semantics.
- HANDSHAKE.md — capability bits and negotiation direction.
- FLOW.md — processing order.

## Release gate

HTTP 4.0 must remain experimental until the conformance suite passes on Linux, macOS, and Windows; parser and reassembly fuzzing is performed; memory and queue limits are enforced in the live server; the transport security contract is implemented and verified; resume checkpoints and file integrity are enforced; and an independent security review is recorded.
