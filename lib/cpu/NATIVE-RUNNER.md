# `lib/cpu` — The OS-Specific Native Runner

This document covers the specialized, OS-specific **native runner** that
executes the `/lib/cpu` machine (an `SLCPU` plus a compiled `.sleela` program)
on a native fast path — as quickly as native code allows — while keeping every
safety and security concern **named in the clear** and checked.

The design principle: running native, OS-specific code fast is only acceptable
if each concern is explicit. Nothing is implicit here. A reviewer can read the
guard top to bottom and know exactly what the native path is permitted to do.

## The two faces of the runner

### 1. SLeeLa source (`lib/cpu/`)

| Class | Role |
|---|---|
| `SLNativeTarget` | Names the OS (`OS_LINUX`/`OS_WINDOWS`/`OS_MACOS`), the architecture/ABI, and the concrete native artifact (`.so`/`.dll`/`.dylib` or a binary) and entry symbol to load — **in the clear**. |
| `SLNativeCapability` | The **capability sandbox**: a deny-by-default set naming exactly what the native path may touch (`CAP_MEMORY`, `CAP_FILE_IO`, `CAP_DYNAMIC_LIBRARY`, …), matching the host contract in `impl/core/sleela_platform.h`. |
| `SLNativeGuard` | The **named safety contract**: trusted SHA-256 verification, the capability sandbox, bounded handles, resource ownership + fail-closed, a wall-clock deadline, cooperative cancellation, and ordered teardown. A runner refuses to start until `satisfied()`. |
| `SLNativeRunner` | The runner: `verify → load → bind → run → teardown`. Runs on the native fast path when the guard is satisfied; falls back to the portable in-Sleela `SLControlUnit` otherwise — *fast where it safely can, correct everywhere*. |

### 2. Native substrate (`impl/core/`)

| File | Role |
|---|---|
| `sleela_cpu_runner.h` / `.c` | The native backend behind the `native sleela_cpu_runner_*` bridge. OS-neutral C that executes the CPU ISA (`[opcode:8][rd:8][rs:8][imm:8]`, exactly as `SLInstruction.encode()`) over a bounded, VM-owned word store, with a self-contained SHA-256 for the trusted-load gate and bounded handle tables. |
| `cpu_runner_smoke.c` | The verified smoke test (wired into `make test-cpu-runner` and `test-platform`). |

## The named safety and security concerns

Each is a field a reviewer can see, not an assumption:

- **Trusted load (verification).** `sleela_cpu_runner_sha256()` measures the
  artifact; `SLNativeGuard.verifyDigest()` matches it against the trusted digest
  **before** the native core is opened. The measured digest is a real SHA-256
  (cross-checked against `sha256sum`). Fail → refuse to load.
- **Capability sandbox (least privilege).** `SLNativeCapability` is
  deny-by-default; `grantRunnerMinimum()` grants only `DYNAMIC_LIBRARY` +
  `MEMORY`. The native `open()` takes a capability **mask** and refuses any
  capability not granted by name. A runner can only ever narrow privilege.
- **Bounded handles.** Every `open`/`create` returns a VM-local integer handle
  (`>0`) into a bounded table — never a raw pointer — the same discipline the
  runtime uses for sockets/files/threads.
- **Resource ownership + fail-closed.** The backend owns each resource and
  releases it on destroy/close; a stale handle is rejected, never reused.
- **Deadline + cancellation.** A run executes in bounded slices
  (`sliceSize`), so the guard's safe point is polled between slices, a
  cooperative cancel is observable, and a wall-clock deadline is enforced — a
  native run can never hang the host unbounded.
- **Ordered, idempotent teardown.** `teardown()` quiesces the runner, closes
  the core, and releases the library handle; safe to call twice.

## Usage

```sleela
// Build and boot a machine (see CPU.md), then compile a program.
SLMachine m = new SLMachine();
m.buildHardware("SleelaWorks", "SL-Core-X", 4, 32, 3200);
m.boot();
SLProgram app = m.compileWithTiming("app.sleela", 0, "app");  // carries a timing manifest

// Run it on the OS-specific native fast path, under the full safety contract.
SLNativeRunner runner = m.runNative(
    app,
    SLNativeTarget.OS_LINUX,       // the host OS
    "@in-process",                 // the built-in native executor (or a .so path)
    "sleela_cpu_entry",            // the entry symbol
    "<trusted-sha256-hex>",        // the digest the artifact must match
    0,                             // load address
    100000,                        // instruction budget
    1000);                         // 1000 ms deadline

if (runner.usedNativeFastPath()) {
    // runner.instructionsRun(), runner.exitStatus()
} else {
    // guard not satisfied or no native core: runner.error() says why, in words
}
```

### Direct lifecycle (explicit control)

```sleela
SLNativeTarget target = new SLNativeTarget();
target.configure(SLNativeTarget.OS_LINUX,
                 SLNativeTarget.ARTIFACT_SHARED_LIBRARY,
                 "@in-process", "sleela_cpu_entry");

SLNativeGuard guard = new SLNativeGuard();
guard.configure(1000);                 // deadline ms; strict defaults
guard.expectSha256("<trusted-hex>");   // the trusted digest
// guard.grant(SLNativeCapability.CAP_TERMINAL);  // widen only by name, if needed

SLNativeRunner runner = new SLNativeRunner();
runner.configure(target, guard);
if (runner.verify() && runner.load() && runner.bindCpu(cpu)
        && runner.bindProgram(app, 0)) {
    runner.run(100000);
}
runner.teardown();                     // always, deterministically
```

## OS specificity

The execution core is OS-neutral C; the OS-specific concern is **loading** the
fast core as the host's native library — `.so` via `dlopen` (Linux), `.dll` via
`LoadLibrary` (Windows), `.dylib` (macOS) — through the single
`impl/core/sleela_library` boundary that already backs the rest of the runtime.
The `@in-process` target binds the built-in native executor, so the runner has a
working native path on every supported OS out of the box, with external native
cores loaded through the same verified, capability-gated door.

## Build & test

```sh
cd impl
make test-cpu-runner      # verify → sandbox → run → teardown, 12 checks
make test-platform        # the cross-OS native smoke suite (includes the above)
```

The smoke test runs the same `2 + 3 → SYS_WRITE('A') → SYS_EXIT` demo program
`SLMachine.loadDemoProgram` builds, confirming the native fast path produces the
identical observable result (`'A'`), exits cleanly, rejects out-of-range access,
refuses to open without the capability, and leaks no native resources.

## Fidelity note

The native core is a faithful, OS-neutral executor of the same ISA the in-Sleela
`SLControlUnit` runs; the two produce identical observable results. The point of
this layer is a native fast path whose every safety and security property —
verification, sandboxing, bounded ownership, time-bounding, teardown — is a
named, auditable concern rather than an implicit assumption.
