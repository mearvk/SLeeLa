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
- Strict DOWNLOAD metadata parsing rejects malformed unsigned integers, extra fields, unsafe path-like filenames, unterminated fixed-size strings, empty fields, and offset greater than total size.
- HTTP request framing rejects duplicate Content-Length, Transfer-Encoding (unsupported by this parser), malformed/oversized Content-Length, undeclared request bodies, body bytes beyond the declared length, and truncated bodies.
- HTTP body limit is aligned with one maximum-size native frame (header plus 16 MiB payload).
- Frame decoder rejects undefined flag bits.
- Bounded per-stream reference state model enforces per-stream sequence, stream byte, aggregate byte, duplicate OPEN, and monotonic/non-reused stream-ID rules.
- Resume-policy gate validates principal/file-version/size/offset/expiry bindings and requires chunk/final-digest verifier callbacks; it is not itself a cryptographic verifier or durable store.
- Normative draft specification, conformance plan, capability matrix, and optional libFuzzer harness.

## Latest CI evidence

- HTTP 4.0 Conformance run **passed** on commit `7a1b867148b039b66b50ae1604e2e005d33da6bf`: Linux, macOS, Windows/MinGW-w64, and the bounded parser fuzz smoke job. See [run 39711902337](https://github.com/mearvk/SLeeLa/actions/runs/39711902337).
- Native C/C++ Portability run **failed** on the same commit on Linux, macOS, and Windows/MinGW-w64. The failure occurred at the repository-wide SHA-256 integrity gate before compilation: mismatches were reported for `impl/frontend/lexer.cpp`, `lib/character/Character.sleela`, and the HTTP 2.0, 3.0, and 4.0 `SLPackage.sleela` files. See [run 39711902376](https://github.com/mearvk/SLeeLa/actions/runs/39711902376).
- Do not regenerate or bless the repository integrity manifest blindly. Each mismatched file must be reviewed against its intended change, then the manifest updated only for approved content and the native workflow rerun.

## Not yet established

- The stream-state model is not wired into the server's frame-dispatch path; the reference endpoint currently handles one frame per HTTP request.
- WINDOW credit enforcement and a complete bidirectional stream/session state machine are not integrated into live dispatch.
- Control-frame payload schemas are not yet comprehensively validated for all nine frame types.
- The server uses HTTP/1.x over TCP and does not provide TLS/QUIC itself. No authenticated HTTP/3/QUIC carrier adapter exists.
- Public binding is opt-in and still requires an authenticated transport boundary; loopback is the safe default.
- No durable authenticated resume store, production token/chunk/final-digest verifier, or crash-safe final publication exists. The resume policy relies on externally supplied verification callbacks.
- No native application-frame MAC/key-rotation implementation exists.
- Capability negotiation is not bound to an authenticated transcript.
- No completed C/SLeeLa shared conformance vectors, long-duration fuzz corpus, production threat-model validation, or independent security review is recorded.

## Release gate

Keep HTTP 4.0 experimental until all of the following are demonstrated:

1. Review and resolve every repository SHA-256 manifest mismatch without bypassing the integrity check.
2. Pass native repository CI and HTTP 4.0 conformance CI on Linux, macOS, and Windows.
3. Integrate per-stream state and receive/send flow-control into the actual persistent server dispatch path, with overflow, reset, duplicate, ordering, and allocation-failure tests.
4. Define and validate exact payload schemas and state rules for every frame type.
5. Implement an authenticated carrier adapter backed by a maintained HTTP/3/QUIC implementation, or explicitly limit deployment to a separately authenticated carrier. Do not claim native QUIC support until it exists and passes interoperability tests.
6. Implement durable, authenticated resume checkpoints, atomic persistence, crash-safe publication, and end-to-end final digest verification using vetted cryptographic primitives.
7. Run sustained coverage-guided fuzzing and attach reproducible logs.
8. Complete an independent security review and publish its scope and findings.

## Documents

- SPEC.md — normative frame and security rules.
- CONFORMANCE.md — required deterministic/adversarial tests.
- CAPABILITY-MATRIX.md — implementation evidence and release gates.
- SECURITY.md — carrier boundary and security requirements.
- RESUME.md — resumability semantics.
- HANDSHAKE.md — capability bits and negotiation direction.
- FLOW.md — processing order.
- PRODUCTION-GATES.md — implementation plan for the remaining production requirements.
