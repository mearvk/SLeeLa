# `@native`, VM dispatch, and Native Drive

This page distinguishes four facilities that must not be conflated: AOT
annotation analysis, typed in-process native dispatch, VM-owned worker handles,
and the external Native IPC worker process.

## 1. The `@native` annotation

`@native` is a marker annotation for ahead-of-time native compilation. It is
behaviour-neutral in ordinary VM execution: it does not authorize a module,
load a library, start a process, or route a method call. The AOT commands remain:

```sh
sleela aot --list source.sleela
sleela aot --emit source.sleela
```

The AOT subset is deliberately narrow and fail-closed. It accepts only the
constructs explicitly supported by the emitter; unsupported constructs are
refused. The emitter is whole-program, not an independent per-method ABI
emitter. `@native` is not an IPC or process-launch annotation.

## 2. Existing syntax-3.0 in-process native dispatch

The VM implements the append-only `OP_NATIVE_OPEN`, `OP_NATIVE_CALL`, and
`OP_NATIVE_CLOSE` operations, exposed as:

- `nativeOpen(path)`: host-authorized shared-library open; returns a
  generation-checked VM-owned handle.
- `nativeCall(handle, method, ...args)`: invokes a named method through the
  versioned typed ABI.
- `nativeClose(handle)`: closes and invalidates the handle.

The ABI supports null, boolean, signed int64, binary64 double, and
length-delimited UTF-8. It never exposes VM arrays, structs, object handles,
pointers, or raw `SLValue` objects. A plugin may return NOT_HANDLED to permit
same-name bytecode fallback; a handled error, stale handle, denied path, or
ABI mismatch is not silently retried in the interpreter.

The host must install an explicit authorization callback. A path identifies a
candidate; it does not grant authority. In-process native code is trusted code,
not a sandbox. Callback names require a separate host allowlist, callbacks are
synchronous, and recursive native operations are rejected.

See [Native Drive](../impl/native-drive/README.md) and the
[loader contract](../os-level-support/loader/README.md).

## 3. VM-owned IPC worker handles

The VM core exposes a bounded table of 64 opaque IPC worker references through
`slvm_ipc_worker_register`, `slvm_ipc_worker_acquire`,
`slvm_ipc_worker_release`, and `slvm_ipc_worker_close`.

Registration accepts an already-created and already-authorized worker plus a
close callback; registration does not launch or authorize a process. Handles
carry a slot and nonzero generation. Leases prevent a worker from being closed
while an operation is using it. Close prevents new acquisitions, waits for
leases to drain, invalidates the generation, and invokes the close callback
outside the table lock. VM shutdown closes registered workers. Registration is
rejected once shutdown begins.

This is a VM ownership/lifecycle API, not yet a source-level worker dispatch
feature. The reserved Native IPC bytecode opcodes remain fail-closed until a
host supervisor adapter and capability policy are connected.

## 4. Native IPC v1 process worker

The repository now includes a persistent Python reference worker and host-side
supervisor in `tools/sleela-ipc-worker.py`. It provides:

- A version-1 `SLIP` framed protocol with a 24-byte little-endian header,
  request IDs, and a 16 MiB payload ceiling.
- A handshake before requests, exact stream reads, scalar request/response
  encoding, and rejection of malformed frames and trailing bytes.
- Persistent child-process ownership using an argv vector and
  `shell=False`, an explicit working directory, an environment allowlist,
  SHA-256 verification of the launched executable, request timeouts, bounded
  stderr capture, and graceful shutdown followed by terminate/kill fallback.
- A reference worker implementing `math.ping`, `math.capabilities`,
  `bigint.add`, `bigint.sub`, `bigint.mul`, and `pi.compute`.

The reference `pi.compute` service uses Python arbitrary-precision integers
and Chudnovsky binary splitting. Its 20,000-fractional-digit test checks the
output length, known prefix, and a pinned SHA-256 digest. This validates the
reference worker's IPC request/response path; it does **not** demonstrate that
SLeeLa's VM math module or `sleelactl` computed those digits.

The C transport-neutral client and the C frame/scalar/request/limb codecs are
separate building blocks. The canonical base-1e9 limb codec is not yet the
payload representation used by the Python reference worker's big-integer
methods, which currently accept decimal strings.

## 5. Required integration gates

1. Connect the process supervisor to the VM-owned worker-handle lifecycle.
2. Add a host-controlled capability/authorization adapter and connect it to
   the reserved bytecode operations; source metadata must never grant authority.
3. Extend integrity policy to cover every executable input that matters,
   including interpreter/script or worker payloads, not only `argv[0]`.
4. Decide and implement the production big-integer wire representation:
   canonical base-1e9 limbs or a separately specified bounded decimal format.
5. Add fault-injection coverage for timeout, cancellation, child crash,
   output flood, malformed response, and cleanup/reaping.
6. Route SLeeLa's actual Pi/math workload through the authorized VM worker path,
   then compare results with an independent reference and record wall time,
   CPU time, peak memory, output hash, commands, and runner details on Linux,
   Windows, and macOS.

The 20,000-digit Python reference-worker test is a correctness fixture, not a
SLeeLa runtime performance result. Do not extrapolate a VM speed-up from it.
