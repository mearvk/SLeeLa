# Pi generator — native IPC bridge implementation status

**Date:** 2026-11-09  
**Branch:** `native-ipc-pi-bridge-2026-11-09`

## Current implementation

The SLeeLa VM already has syntax-3.0 `nativeOpen`, `nativeCall`, and
`nativeClose` bytecode operations for authorized **in-process** shared
libraries. The scalar ABI supports null, bool, int64, binary64, and UTF-8.
It deliberately does not expose VM arrays, structs, pointers, or raw handles.

This work adds the first isolated-process wire-format slice:

- `impl/native-drive/native_ipc_protocol.h`: 24-byte version-1 frame header,
  explicit little-endian encoding, bounded payloads, scalar values, and
  request method/argument encoding.
- `impl/tests/native_drive/native_ipc_protocol_smoke.c`: header, int64,
  binary64, UTF-8, invalid-boolean, and truncated-value checks.
- `.github/workflows/native-drive.yml`: runs the codec smoke test on
  Linux, macOS, and Windows in the manually dispatched Native Drive workflow.
- `impl/native-drive/IPC-PROTOCOL.md`: wire, limb, worker, security, and
  six-workstream implementation contract.

## Not yet implemented

The codec is not a worker transport. Still required are the request/response
payload codec, persistent worker process, stdin/stdout pipe supervisor,
canonical signed base-1e9 limb payloads, timeout/cancellation/backpressure,
child cleanup/reaping, and the SLeeLa VM process-call opcode/API. The existing
`sleelactl run math` reference subject is not yet a persistent external worker.

The 20,000-digit Pi workload has **not** been routed through this IPC path.
Existing benchmark results remain separate and must not be represented as an
IPC benchmark.

## Verification status

The workflow is `workflow_dispatch`-only. The new cross-platform codec test
is configured but has not yet been executed in this work session. No PASS
claim is made until its run completes. Next gate: run the Native Drive workflow,
fix any compiler/platform issues, then add process-level framing tests before
connecting big-integer limb operations.
