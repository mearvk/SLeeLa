# Native IPC Pi 20,000-digit verification record

**Record date:** 2026-11-09  
**Repository:** `mearvk/SLeeLa`  
**Worker implementation:** [25dfdb2](https://github.com/mearvk/SLeeLa/commit/25dfdb2880aa5a2afdceb504620fea072804b001)

## Purpose and scope

This record captures the 20,000-fractional-digit correctness fixture for the
Python Native IPC reference worker. It is intentionally **not** presented as
a performance benchmark for SLeeLa's VM, `sleelactl`, or native big-integer
kernel.

The reference worker's `pi.compute` method uses Python arbitrary-precision
integers and Chudnovsky binary splitting. It receives a scalar integer digit
count and returns the formatted decimal expansion through the Native IPC v1
request/response channel.

## Reproduction command

From the repository root, with Python 3.12 or a compatible Python 3 runtime:

```sh
python -m unittest discover -s tools/tests -p "test_sleela_ipc_worker.py" -v
```

The focused test is
`IPCWorkerTests.test_twenty_thousand_pi_digits_match_reference_hash`.
The Native Drive GitHub Actions workflow configures this test on
`ubuntu-latest`, `macos-latest`, and `windows-latest` using Python 3.12.

## Recorded correctness assertions

| Assertion | Expected value |
|---|---|
| Requested fractional digits | 20,000 |
| Returned string length | 20,002 characters, including `3.` |
| Prefix | `3.14159265358979323846264338327950288419716939937510` |
| SHA-256 of complete returned string | `977f8d4251b4f633100b4d2dbc214b874c45353b0e3debe41046753777fd6efc` |

The test checks all three assertions. The digest is a pinned expected value in
the test source; the test itself does not document the provenance of the
independent reference file. That provenance should be recorded before treating
the digest as a fully audited independent-reference comparison.

## What this establishes

- The host can launch a persistent child worker through an argv vector.
- The worker completes a protocol handshake and handles multiple requests.
- The 20,000-digit request returns a result with the expected length and
  known prefix, and matches the pinned complete-output digest.
- The test also exercises executable-integrity rejection, frame-size bounds,
  and malformed frame magic.

## What this does not establish

- It does not show SLeeLa bytecode dispatch to the worker. The reserved Native
  IPC opcodes remain fail-closed pending a host supervisor/capability adapter.
- It does not show `sleelactl`'s math module or SLeeLa's VM big-integer kernel
  computed the result. The reference worker uses Python's bigint implementation.
- It does not show base-1e9 Pi limb arrays being sent through this worker.
  The current Python reference methods use decimal-string scalar arguments;
  the canonical limb codec is a separate C codec component.
- It does not establish an IPC speed-up, throughput, latency, CPU usage, or
  peak-memory result.
- It does not prove that a particular Actions run passed on all three OSes.
  The workflow matrix is configured, but a specific run URL and job
  conclusions must be attached after inspecting the run.

## Performance measurements still required

For a real performance report, record each run's:

1. Exact commit SHA and Actions run URL.
2. OS image, architecture, Python/runtime versions, CPU model, and memory.
3. Exact command and digit count.
4. Cold-start and warm-request wall time, separating process startup from
   compute time.
5. CPU time and peak resident memory.
6. Output length, SHA-256, and independent-reference comparison result.
7. Repeated-run count, median, spread, and any failed or timed-out runs.

The performance comparison must use the intended SLeeLa `sleelactl`/VM
math-module integration as well as the reference worker, with the same
20,000-digit correctness target. Do not derive a runtime speed-up from the
Python reference worker's result.
