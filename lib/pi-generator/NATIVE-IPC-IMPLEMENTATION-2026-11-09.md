# Pi generator — Native IPC implementation status

**Status date:** 2026-11-09  
**Repository branch:** `master`  
**Reference implementation commit:** [25dfdb2](https://github.com/mearvk/SLeeLa/commit/25dfdb2880aa5a2afdceb504620fea072804b001)

## Implemented components

- **Wire framing and codecs:** 24-byte v1 header, explicit little-endian
  fields, request IDs, 16 MiB payload bound, message-kind validation, scalar
  values, request payloads, and canonical signed base-1e9 limb encoding.
- **C transport client:** transport-neutral client in
  `impl/native-drive/native_ipc_client.c/.h`, using caller-provided exact-read
  and exact-write callbacks.
- **VM ownership:** bounded 64-slot worker handle table with generation-
  protected handles, lease-based acquire/release, close-after-lease-drain,
  duplicate/stale-handle rejection, and VM-shutdown cleanup.
- **Persistent process worker:** `tools/sleela-ipc-worker.py` contains a
  Python host-side supervisor and reference worker. It performs a protocol
  handshake, launches with argv and `shell=False`, verifies the launched
  executable SHA-256, applies an explicit working directory and environment
  allowlist, uses request timeouts, bounds stderr capture, and terminates or
  kills a child that does not shut down cleanly.
- **Reference methods:** `math.ping`, `math.capabilities`, decimal-string
  `bigint.add`, `bigint.sub`, `bigint.mul`, and `pi.compute`.
- **20,000-digit correctness fixture:** the worker test checks a 20,000-digit
  Pi result's length, known prefix, and pinned SHA-256 digest.
- **CI integration:** `.github/workflows/native-drive.yml` includes Python
  3.12 worker tests on Ubuntu, macOS, and Windows, alongside the C codec/client
  smoke tests and POSIX VM-handle lifecycle test.

## Workstream status and boundaries

| Workstream | Status |
|---|---|
| AOT annotation | `@native` remains AOT metadata only; it does not request or authorize a worker. |
| Scalar ABI and wire codec | Scalar ABI v1 remains stable. C request and canonical base-1e9 limb codecs are implemented. |
| C transport client | Implemented as a transport-neutral client with caller-supplied exact I/O callbacks. |
| VM worker ownership | 64-slot generation-protected handle table and lease/shutdown lifecycle are implemented. |
| Process supervision | A persistent Python reference supervisor/worker exists with handshake, executable hash check, bounded stderr, timeout, and child cleanup. |
| VM bytecode integration | Reserved Native IPC opcodes remain fail-closed. The host supervisor is not yet connected as a bytecode OPEN/CALL/CLOSE route. |
| Pi integration | The Python reference worker computes Pi. SLeeLa's own `sleelactl` math module and VM big-integer path are not yet demonstrated as clients of this worker. |
| Production limb transport | The C limb codec exists, but the Python reference worker's big-integer methods currently use decimal-string scalar arguments. |
| Performance benchmark | No reliable end-to-end IPC timing, CPU, or peak-memory result is recorded in this report. |

## Worker test and reproducible command

The workflow installs Python 3.12 and runs:

```sh
python -m unittest discover -s tools/tests -p "test_sleela_ipc_worker.py" -v
```

The test suite covers handshake and multiple requests, bigint add/multiply
examples, a 50-digit Pi known-answer case, the 20,000-digit Pi digest check,
executable-integrity rejection, payload bounds, and malformed frame magic.

The 20,000-digit fixture expects a 20,002-character result including the
leading `3.`, checks the prefix
`3.14159265358979323846264338327950288419716939937510`, and pins this
SHA-256 over the complete returned string:

```text
977f8d4251b4f633100b4d2dbc214b874c45353b0e3debe41046753777fd6efc
```

The Python worker's `pi.compute` implementation uses Chudnovsky binary
splitting with Python's arbitrary-precision integers. The fixture therefore
validates the reference service and its IPC transport, not SLeeLa's native
math kernel, `sleelactl`, or a 750x runtime claim.

## CI scope and evidence

The Native Drive workflow matrix is configured for `ubuntu-latest`,
`macos-latest`, and `windows-latest`. It runs the Python worker tests on all
three. The C protocol/client smoke tests have POSIX and Windows steps; the VM
worker-handle lifecycle test is currently POSIX-only.

A workflow definition is not proof that a particular run passed. Record a
specific Actions run URL and its three job conclusions here only after checking
the run itself. The current documentation update does not claim a new CI pass.

## Remaining work

1. Connect the host supervisor to the VM-owned worker handle table through a
   host-controlled authorization/capability adapter.
2. Wire the reserved bytecode operations only after the host policy and
   lifecycle contract are complete; preserve fail-closed behavior meanwhile.
3. Define whether production big integers cross as canonical base-1e9 limbs
   or a bounded decimal-string format, then use one contract consistently.
4. Pin and verify all executable inputs needed to start a worker, not merely
   the first executable path.
5. Add process fault-injection tests for timeout, cancellation, crash,
   malformed/oversized output, output flood, and shutdown races.
6. Run SLeeLa's actual Pi workload through the intended `sleelactl`/VM path,
   verify all requested digits against an independently sourced reference,
   and record wall time, CPU time, peak memory, environment, command, output
   length, and hash on all three operating systems.
