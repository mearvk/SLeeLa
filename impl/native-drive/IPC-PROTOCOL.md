# Native IPC v1 — Pi math worker boundary

**Implementation status:** The portable 24-byte frame codec and scalar-value
codec are implemented in `native_ipc_protocol.h`; the focused C smoke test is
`native_ipc_protocol_smoke.c`. The persistent worker, pipe supervisor,
request-payload codec, canonical big-integer limb messages, and Pi integration
remain in progress. This is not yet an end-to-end IPC claim.

## Why a second boundary is needed

SLeeLa syntax 3.0 already has host-authorized `nativeOpen`,
`nativeCall`, and `nativeClose` bytecode operations. They call a typed
shared-library ABI **inside the VM process**. That ABI deliberately excludes
arrays, structs, object handles, pointers, and raw `SLValue` objects. The
Pi generator needs large base-1e9 limb arrays, so the next step is a separately
spawned math worker with explicit serialization—not widening the in-process
ABI to leak VM memory.

## Frame layout

All fields are encoded explicitly in little-endian order; C struct layout is
never sent over the wire. Header size is 24 bytes:

| Offset | Width | Field |
|---:|---:|---|
| 0 | 4 | Magic `SLIP` |
| 4 | 2 | Version (1) |
| 6 | 2 | Kind: request=1, response=2, cancel=3, hello=4, hello-ack=5 |
| 8 | 8 | Request ID |
| 16 | 4 | Payload length |
| 20 | 4 | Signed status |

Maximum payload is 16 MiB. The codec reuses the existing `SLNativeValue`
tags: null, boolean, signed int64, binary64 double, and length-delimited UTF-8.
Strings are borrowed byte spans while decoding and must be copied before the
receive buffer is reused; validate UTF-8 before constructing a SLeeLa String.
Unknown tags, bad magic/version/kind, truncated values, invalid booleans, and
oversized payloads are rejected.

## Next payload contract

A request payload will contain: uint16 method-name length, UTF-8 method bytes,
uint16 argument count, then the encoded arguments. A successful response
contains one encoded value; an error response uses a stable status and an
optional bounded UTF-8 diagnostic. Reject empty names, invalid UTF-8,
duplicate outstanding request IDs, trailing bytes, and count/length mismatch.

Pi-specific operations will use a dedicated limb representation:
- base: 1,000,000,000;
- limbs: unsigned 32-bit little-endian, least-significant limb first;
- explicit limb count and sign;
- zero has one zero limb and positive sign;
- no redundant high zero limbs;
- declared count and byte length must agree and stay within configured limits.

Initial methods: `math.ping`, `math.capabilities`, `bigint.add`,
`bigint.sub`, `bigint.mul`, `bigint.divmod`, and `bigint.isqrt`.
The worker must report its protocol and arithmetic-kernel versions during
handshake. A method result must be differential-tested against the VM kernel.

## Process and security contract

The first transport is a private child process using stdin/stdout pipes.
Launch with an argv vector, never a shell string. Stderr is diagnostic-only
and separately bounded. The supervisor owns the child, request IDs, deadlines,
and buffers. Enforce exact executable authorization and integrity policy,
explicit working directory and environment allowlist, bounded input/output,
startup and request timeouts, cancellation, child termination and reaping,
and cleanup when the VM is destroyed.

Source annotations can request a mode but cannot authorize it. The host must
intersect requested capabilities with its own immutable policy. A worker
failure after a request is accepted is an error, not a silent interpreter
fallback. Fallback is safe only for an explicit `NOT_HANDLED` response before
the operation has performed side effects.

## Six workstreams

1. **Annotation/lowering:** preserve `@native` as AOT metadata; add explicit
   process-execution metadata and reject unsupported signatures.
2. **Typed ABI:** retain scalar ABI v1; add canonical, bounded limb messages.
3. **VM dispatch:** reuse existing native opcodes; add a distinct worker-call
   boundary so in-process module semantics remain unchanged.
4. **IPC:** complete request/response payloads, pipe transport, framing,
   backpressure, timeout, cancellation, and malformed-peer handling.
5. **Lifecycle/security:** authorization, integrity, environment/workdir
   policy, child reaping, and VM-shutdown cleanup.
6. **Conformance:** codec tests, process fault injection, differential limb
   arithmetic, and 20,000-digit Pi validation on Linux, Windows, and macOS.

## Verification

The codec smoke test checks header round-trip and field endianness, signed
int64, binary64, UTF-8 span round-trip, invalid boolean rejection, and
truncated-value rejection. It does not test a process transport. The existing
native-call smoke suite tests the in-process typed ABI, source dispatch,
fallback, callbacks, authorization, and reentrancy; it does not prove IPC.

Before routing Pi through the worker, add tests for fragmented reads/writes,
multiple frames per read, malformed/oversized frames, invalid UTF-8, duplicate
IDs, timeout/cancel, worker crash, output flood, authorization denial,
cleanup/reaping, and cross-platform behavior. Then compare each limb operation
with the VM implementation and verify all 20,000 fractional digits against an
independent reference, recording accuracy, wall time, CPU time, memory, and
output hash.

The current `@native` annotation is not an IPC directive. It does not start a
worker, grant process capabilities, or convert VM arrays into wire buffers.
