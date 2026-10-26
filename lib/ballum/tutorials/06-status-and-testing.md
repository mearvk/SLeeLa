# Lesson 6 — Status Codes and Testing

Ballum uses explicit result codes so an application can distinguish invalid input from missing platform support or a native operation failure. The exact meaning of a code should come from the package source for the version being built. Preserve the returned status instead of collapsing every nonzero result into a generic “failed” message.

## Common status codes

| Code | Meaning | First response |
|---:|---|---|
| 0 | OK | Record success only after the call returns this status. |
| 1 | Invalid argument | Inspect IDs, ranges, null/empty values, and expected encodings. |
| 2 | Backend unavailable | Check backend compilation, selection, session type, and host support. |
| 3 | Backend error | Preserve diagnostic context and investigate the native layer. |
| 4 | Display not found | Re-enumerate and verify the display ID. |
| 5 | Out of bounds | Recheck dimensions and coordinate arithmetic. |
| 6 | Buffer too small | Confirm the required capacity and the actual supplied size. |
| 7 | Unsupported capability | Skip the feature or use a supported alternative. |
| 8 | Permission denied | Follow the operating system's documented permission flow. |
| 9 | Not initialized | Check initialization order and object lifetime. |
| 100 | SLeeLa-side bridge required | Verify that the program is using the real VM bridge rather than a model-only path. |

Use the package's status-name helper when available, but keep the numeric code in logs as well. A readable name is helpful for people; the number is useful for comparing reports across environments.

## Test categories

A robust test suite includes positive cases, negative cases, environmental cases, and integration cases.

### 1. Input validation

- Valid and invalid display IDs.
- Zero or invalid dimensions if the API permits them to reach the boundary.
- Negative coordinates.
- Coordinates exactly equal to width or height.
- Values just below, at, and just above each documented control limit.
- Incorrect gamma scaling.
- Buffer sizes below the required amount.

Expected result: invalid inputs are rejected consistently, without memory errors, out-of-range writes, or misleading success logs.

### 2. Environment and capability

- No display or a headless CI runner.
- Display present but pixel operations unsupported.
- Control capability absent on one display and present on another.
- Backend unavailable or not compiled into the runtime.
- Permission denied by the operating system.
- Display removed or reconfigured between enumeration and operation, where this can be safely simulated.

Expected result: unsupported conditions are reported cleanly, and the program can continue or exit according to its documented policy.

### 3. Input polling

- No input event ready.
- One event.
- A burst of events.
- An error from the poll path.
- Maximum event limit reached.
- Unsupported platform/input backend.

Expected result: polling terminates predictably, does not spin forever, and never reads event fields after a failed poll.

### 4. Integration

- SLeeLa source compiles with the intended compiler version.
- The VM build includes the Ballum bridge.
- The native manager tests pass.
- The backend selected at runtime matches the target operating system.
- A live operation is verified on supported hardware, with permissions and settings documented.

## Test layers are not interchangeable

A unit test of range validation does not prove a platform backend works. A native manager test does not prove the VM bridge is linked. A successful SLeeLa compile does not prove the operating system accepted a request. Keep separate results for compilation, unit tests, bridge integration, and real-device smoke tests.

The native manager test source is documented in the project as `tests/test_ballum_manager.c`; verify its current build instructions and test target before running it. Do not report tests as passing unless they were actually executed and their output inspected.

## A useful bug report

Include the OS/version, compiler/runtime version, commit or build identifier, graphical/headless session, bridge/backend configuration, display geometry and capability mask, operation attempted, numeric status, readable status name, and a minimal reproduction. Exclude unnecessary personal input data.

## Practice

Use [05-headless-safe.sleela](../examples/05-headless-safe.sleela) to think through unavailable-host behavior, then review [02-safe-pixel.sleela](../examples/02-safe-pixel.sleela) for a negative bounds test. Confirm which tests you actually ran; label unexecuted tests as pending.

**Checkpoint:** explain the difference between backend unavailable, unsupported capability, permission denied, and a native backend error.