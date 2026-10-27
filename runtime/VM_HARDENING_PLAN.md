# Runtime and VM Hardening Plan

Status: implementation plan for the runtime/SLVM integration. This document is not a claim that the controls below are already implemented.

## 1. Garbage collector correctness

- Preserve remembered old-generation owners across young collections until the owner is reclaimed or a precise edge scan proves it no longer points to young objects.
- Treat mark-stack allocation failure as a failed trace. Never sweep after an incomplete mark.
- Check capacity growth and byte-accounting arithmetic for `SIZE_MAX` overflow.
- Add regression tests for old-to-young references across repeated young collections, cycles, roots, external payload ownership, incremental stepping, and allocation failure.
- Define write-barrier semantics explicitly (SATB deletion barrier versus incremental-update insertion barrier); do not mix their assumptions.

## 2. Security policy consistency

- Keep unknown class names denied by default.
- Make the SLeeLa-facing `runtime_security.sleela` contract consistent with `SECURITY_SUPERVISOR.md`; permissive placeholder methods must not be described as enforcement.
- Require class authorization and quota reservation before object/service admission, and release quota on all failure and destruction paths.
- Test denied classes, per-class quotas, aggregate role quotas, overflow-safe quota arithmetic, and cleanup after failed allocation.

## 3. Recursion, roots, and execution boundary

- Enumerate live VM roots at safepoints: globals/constants, operand stack, active call frames and locals, closure captures, exception/unwind state, and native handles.
- Keep objects in active recursive frames reachable for the duration of the frame.
- Specify the native-call boundary and ownership contract for externally allocated payloads.
- Add integration tests for recursive functions returning class-level objects, deep-but-bounded recursion, collection during recursion, and exceptions/unwind while references remain live.

## 4. Version and memory configuration

- Establish one authoritative VM version registry consumed by runtime config, VM summaries, compiler validation, runner generation, and versioned directories.
- Current runtime config accepts versions 1–11. Do not silently enable version 12 until its semantics, sources, and tests exist.
- Separate the collection trigger from the managed-heap hard limit. A 1 MiB default GC threshold is not a 512 MiB memory ceiling.
- Make the desired 512 MiB default lazy (no eager reservation), configurable, and enforced at allocation boundaries.
- Specify Dynamic Memory Guard modes: Hard (fail closed at the limit), Slow Careful (collect/retry within bounded work), and Aggressive RAM (earlier/more frequent collection while still respecting the hard limit).

## 5. Integrated tests, build, and docs

- Add a portable runtime test target and wire it into the repository's supported build/CI paths.
- Test runtime APIs independently, then test the actual SLVM runner with compiled artifacts to prove the runtime is invoked.
- Add deterministic tests for GC, security, configuration parsing, recursion/root enumeration, memory limits, native calls, and error propagation.
- Document which APIs are implemented in C/C++, exposed to SLeeLa, or still placeholders.
- Update `api/runtime/gc/` with lifecycle, root registration, child traversal, barriers, safepoints, failure behavior, and ownership examples.
- Verify Linux, Windows, and macOS build paths before claiming portability.

## Acceptance criteria

A change is complete only when the targeted regression test fails on the old behavior and passes on the corrected behavior; the full runtime and VM test suites pass; and an end-to-end test proves a compiled SLeeLa artifact runs through SLVM with runtime services active.
