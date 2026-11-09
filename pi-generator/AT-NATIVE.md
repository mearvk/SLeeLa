# `@native`, VM dispatch, and Native Drive

This page distinguishes three facilities that must not be conflated:
AOT annotation analysis, typed in-process native dispatch, and the emerging
isolated-process IPC route for Pi's large-integer work.

## 1. The `@native` annotation

`@native` is a marker annotation for ahead-of-time native compilation.
It is behaviour-neutral in ordinary VM execution: it does not authorize a
module, load a library, start a process, or route a method call. The AOT
commands remain:

```sh
sleela aot --list source.sleela
sleela aot --emit source.sleela
```

The AOT subset is deliberately narrow and fail-closed. It currently accepts
scalar literals/variables, arithmetic and comparison, assignment, free calls,
if/while/for, return, and print. Arrays, object construction, qualified calls,
and other unsupported constructs are refused. The emitter is whole-program,
not an independent per-method ABI emitter.

## 2. Existing syntax-3.0 native dispatch

The VM already implements the append-only opcodes `OP_NATIVE_OPEN`,
`OP_NATIVE_CALL`, and `OP_NATIVE_CLOSE`, exposed as:

- `nativeOpen(path)`: host-authorized open; returns a generation-checked
  VM-owned handle.
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

## 3. Why Pi needs a separate IPC route

The 20,000-digit Pi generators use VM-owned arrays and big-integer operations.
Those arrays cannot cross the scalar ABI, and widening the ABI to pass pointers
or VM handles would violate the ownership boundary. The next route is an
isolated math worker process, with canonical serialization of base-1e9 limbs.

The first IPC implementation slice now exists:
- `impl/native-drive/native_ipc_protocol.h`: 24-byte version-1 frame header
  and scalar-value codec, field-by-field little-endian.
- `impl/tests/native_drive/native_ipc_protocol_smoke.c`: focused codec test.
- `impl/native-drive/IPC-PROTOCOL.md`: request/response, limb, lifecycle,
  security, and verification contract.

This is only the wire-codec foundation. A persistent worker, request-payload
codec, process supervisor, canonical limb payloads, and Pi integration remain
in progress. No end-to-end IPC benchmark is claimed.

## 4. Required implementation gates

1. Annotation/lowering metadata must identify the method and requested mode;
   source metadata must never grant authority.
2. Keep scalar ABI v1 stable; add bounded, canonical signed limb encoding.
3. Add a separate VM-to-worker call boundary without changing in-process
   module semantics.
4. Complete request/response framing, pipes, backpressure, timeout, and cancel.
5. Enforce executable integrity/allowlists, environment/workdir policy,
   child cleanup/reaping, and VM-shutdown lifecycle.
6. Test codec and process faults, differential limb arithmetic, then run the
   20,000-digit Pi workload against an independent reference on all three OSes.

The exact wire and test contract is in
[IPC-PROTOCOL.md](../impl/native-drive/IPC-PROTOCOL.md).

## 5. Existing Native Drive

Native Drive supports C/C++ builds through the selected host compiler,
supported SLeeLa AOT output, shared-library ABI checks, shell-free argv process
execution, bounded stdout/stderr capture, process piping, timeouts, and XClass
delegation. The `sleelactl` math subject is currently an in-process native
subject/reference-demo route; it is not yet a persistent external IPC worker
that accepts Pi limb arrays.
