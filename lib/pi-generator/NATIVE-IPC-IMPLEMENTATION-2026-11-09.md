# Pi generator — native IPC bridge implementation status

**Date:** 2026-11-09  
**Branch:** `native-ipc-pi-bridge-2026-11-09`

## Implemented in this branch

- **Wire framing:** 24-byte v1 header, explicit little-endian fields, request IDs,
  bounded 16 MiB payloads, and message-kind validation.
- **Scalar ABI codec:** null, bool, int64, binary64, and borrowed UTF-8 spans;
  malformed tags, invalid booleans, and truncated values are rejected.
- **Request codec:** bounded method names, argument count, scalar argument
  encoding, and strict trailing-byte rejection.
- **Pi limb codec:** signed base-1e9 limbs, least-significant limb first,
  bounded count, range checks, and canonical high-limb/zero rules.
- **Conformance source:** smoke coverage for headers, scalar values, request
  payloads, and canonical limb payloads.
- **Integrity:** refreshed only the 13 SHA-256 entries that failed the previous
  three-OS CI, preserving the other 12,473 entries and the fail-closed verifier.
- **CI:** Native Drive now runs on Linux, macOS, and Windows for relevant source
  changes and for changes to the trusted-source manifest.

## Workstreams 1–5: current boundary

| Workstream | Current state |
|---|---|
| 1. Annotation/lowering metadata | Existing `@native` remains AOT metadata only. A distinct process-worker annotation and compiler/lowering representation are **not yet implemented**. |
| 2. Typed ABI | Scalar ABI v1 remains unchanged; bounded request and canonical base-1e9 limb wire encodings are implemented. |
| 3. VM dispatch | Existing in-process native opcodes are unchanged. A separate VM-to-worker dispatch opcode/API is **not yet implemented**. |
| 4. IPC transport | Frame, scalar, request, and limb codecs are implemented. Persistent child process, pipe framing, partial I/O, backpressure, timeout, and cancellation are **not yet implemented**. |
| 5. Lifecycle/security | Authorization, integrity, environment/workdir policy, bounded output, child reaping, and VM-shutdown cleanup are specified. A worker supervisor enforcing them is **not yet implemented**. |

## Verification and Pi benchmark boundary

The latest three-OS Native Drive workflow is run
[42698637600](https://github.com/mearvk/SLeeLa/actions/runs/42698637600).
Its status must be read from GitHub Actions; no result is assumed here.

The 20,000-digit Pi workload has **not** been routed through this IPC path.
Existing VM/native arithmetic measurements are not IPC measurements. Do not
claim IPC accuracy or speed until the worker is integrated and the result is
checked against an independent 20,000-digit reference.

## Next implementation gate

Implement the compiler metadata and VM worker-call boundary, then a supervised
persistent process transport with fragmented-I/O tests and fail-closed
authorization. Only after those gates should the Pi limb operations be routed
through the worker and benchmarked on all three operating systems.
