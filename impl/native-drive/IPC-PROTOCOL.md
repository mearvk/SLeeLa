# Native IPC v1 — Pi math worker boundary

**Status:** The repository contains the v1 wire codec, transport-neutral C
client, VM-owned worker-handle lifecycle, and a persistent Python reference
worker/supervisor. The pieces are not yet a fully connected SLeeLa bytecode
worker-dispatch feature.

## Architecture and ownership

There are four distinct layers:

1. **Wire codec:** encodes and validates the 24-byte frame header, scalar
   values, request payloads, and canonical signed base-1e9 limbs.
2. **C transport client:** `native_ipc_client.c/.h` performs handshake and
   request/response exchanges through caller-provided exact-read and
   exact-write callbacks. It does not itself create a process.
3. **Python reference supervisor:** `tools/sleela-ipc-worker.py` launches a
   persistent child process, verifies the launched executable hash, applies
   cwd/environment policy, handles request deadlines, captures bounded
   stderr, and owns shutdown/termination.
4. **VM worker ownership:** the VM core provides 64 opaque worker slots with
   generation-protected handles and leases. Registration accepts an already
   created and authorized worker; it is not a launch or authorization path.

The VM handle API is exposed through `slvm_ipc_worker_register`,
`slvm_ipc_worker_acquire`, `slvm_ipc_worker_release`, and
`slvm_ipc_worker_close`. Close prevents new leases, waits for existing leases
to drain, invalidates the generation, and invokes the close callback outside
the table lock. VM shutdown closes registered workers, and new registration is
rejected once shutdown begins.

The reserved Native IPC bytecode opcodes remain fail-closed. The supervisor
adapter and capability policy that would connect bytecode dispatch to the
process worker are not yet complete. Do not treat handle registration as
process authorization.

## Why a separate boundary is needed

SLeeLa syntax 3.0 already has host-authorized `nativeOpen`,
`nativeCall`, and `nativeClose` operations for a typed shared-library ABI
**inside the VM process**. That ABI deliberately excludes arrays, structs,
object handles, pointers, and raw `SLValue` objects. Large Pi integers need a
separate process boundary with explicit serialization—not pointers into VM
memory.

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

Maximum payload is 16 MiB. The scalar codec supports null, boolean, signed
int64, binary64 double, and length-delimited UTF-8. Strings decoded from
borrowed spans must be copied before their receive buffer is reused; validate
UTF-8 before constructing a SLeeLa String. Unknown tags, bad magic/version/
kind, truncated values, invalid booleans, and oversized payloads are rejected.

## Request and big-integer payloads

The request payload is a uint16 method-name length, method bytes, uint16
argument count, then encoded scalar arguments. The implementation rejects
empty/oversized names, malformed UTF-8, truncated values, and trailing bytes.
Successful responses contain one encoded scalar value; nonzero status carries
a bounded UTF-8 diagnostic.

The C codec also defines a canonical signed base-1e9 representation:

- Base: 1,000,000,000.
- Limbs: unsigned 32-bit little-endian, least-significant limb first.
- Explicit limb count and sign (positive=1, negative=2 on wire).
- Zero has one zero limb and positive sign.
- Redundant high zero limbs are forbidden.
- Count, byte length, limb range, and configured maximums are checked.

**Current integration distinction:** the Python reference worker's
`bigint.add`, `bigint.sub`, and `bigint.mul` methods currently accept
decimal-string scalar arguments. The canonical base-1e9 limb codec is not yet
the payload path used by those methods. Do not claim that Pi limb arrays have
already crossed the worker boundary.

Reference methods currently include `math.ping`, `math.capabilities`,
`bigint.add`, `bigint.sub`, `bigint.mul`, and `pi.compute`. The worker's
Pi implementation uses Python arbitrary-precision integers and Chudnovsky
binary splitting.

## Process and security contract

The Python supervisor launches with an argv vector and `shell=False`, verifies
the SHA-256 digest of the resolved `argv[0]`, uses an explicit working
directory and environment allowlist, and captures stderr up to a configured
bound. It applies a request timeout and on shutdown sends a cancel frame,
waits briefly, then terminates or kills and reaps the child if needed.

The current integrity check covers the launched executable path. When the
worker is an interpreter plus script, the script and any other executable
inputs also need an explicit integrity policy before this is treated as a
production authorization boundary. The host must own the allowlist and policy;
source annotations cannot grant capabilities.

A worker failure after a request is accepted is an error, not a silent
interpreter fallback. Fallback is safe only for an explicit NOT_HANDLED
response before the operation has performed side effects.

## Verification

The Native Drive workflow runs:

```sh
python -m unittest discover -s tools/tests -p "test_sleela_ipc_worker.py" -v
```

That suite checks handshake and multiple requests, scalar bigint examples,
50-digit Pi output, a 20,000-digit Pi output length/prefix/SHA-256 fixture,
executable-integrity rejection, payload bounds, and malformed frame magic.

The 20,000-digit fixture validates the Python reference worker. It does not
prove SLeeLa VM/`sleelactl` integration or provide a performance benchmark.
The C codec/client smoke tests exercise their own interfaces; the VM handle
lifecycle smoke test exercises ownership and leases, not process transport.

Before production bytecode integration, add fault tests for fragmented reads
and writes, multiple frames per read, malformed/oversized frames, invalid
UTF-8, duplicate outstanding IDs, timeout/cancel, worker crash, output flood,
authorization denial, executable/script tampering, cleanup/reaping, and
cross-platform behavior. Then differential-test the chosen big-integer
representation against the VM kernel and run the actual SLeeLa Pi workload
against an independent 20,000-digit reference.
