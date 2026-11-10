# SLeeLa Native Drive

The Native Drive is the explicit host-toolchain and process boundary for native
source execution. It connects existing SLeeLa facilities without claiming that
the model frontends under lib/cpu are production C/C++ parsers.

## Implemented

tools/sleela-native-drive.py provides:

- Direct C and C++ compilation through the configured host compiler.
- Multi-translation-unit C/C++ builds (all units in one invocation use one language).
- Executable or shared-library output.
- Optional shared-library export validation (both sl_native_abi_version and
  sl_native_run symbols present) before an artifact is published. The loader
  checks the ABI version value when a trusted module is actually opened.
- SLeeLa @native AOT input through the existing sleela aot --emit contract,
  followed by a host C compiler. The current emitter covers the whole program
  when its C backend can translate it; it is not yet a method-only ABI emitter.
- Argument-vector process execution without an implicit shell.
- Optional stdin-file input, stdout/stderr capture, multi-stage process piping, timeouts,
  and exit-code propagation. Captured output is capped at 16 MiB by default
  across stdout and stderr; `--max-output-bytes` configures the combined limit.
  Exceeding the cap terminates the child process(es), drains the pipes, emits only
  the retained prefix, and returns exit code 6.
- XClass info/emit/run delegation to the production sleela xclass loader.

Build output is staged beside the destination and atomically replaced only
after compilation and optional ABI checks succeed. Compiler selection uses
CC/CXX or --compiler. Extra compiler/linker arguments are passed as individual
argv elements using repeatable --compiler-arg.

## Usage

    # Direct C and C++ (one or more translation units)
    python3 tools/sleela-native-drive.py build main.c math.c -o build/app
    python3 tools/sleela-native-drive.py build main.cpp math.cpp -o build/app

    # Shared library; require the SLeeLa loader ABI before publishing
    python3 tools/sleela-native-drive.py build plugin.c --kind shared \
      --require-sleela-abi -o build/plugin.so

    # Supported @native SLeeLa AOT subset
    python3 tools/sleela-native-drive.py build hot.sleela --language sleela -o build/hot

    # Existing XClass loader; structural reconstruction is not native-code emission
    python3 tools/sleela-native-drive.py xclass --mode info Example.xclass
    python3 tools/sleela-native-drive.py xclass --mode emit Example.xclass
    python3 tools/sleela-native-drive.py xclass --mode run Example.xclass

    # Process invocation and a stdout-to-stdin pipeline
    python3 tools/sleela-native-drive.py run --capture --max-output-bytes 1048576 --timeout 10 -- build/app arg1
    python3 tools/sleela-native-drive.py pipe --max-output-bytes 1048576 --timeout 10 -- producer arg :: filter arg :: consumer arg
    sleelactl run native-drive pipe --max-output-bytes 1048576 -- producer arg :: filter arg :: consumer arg

## What the frontends mean

C and C++ are parsed and compiled by the selected system compiler (Clang, GCC,
or a compatible platform toolchain). This is a real production frontend path
for those languages, but it does not lower their full language grammars into
SLeeLa SLIR. The lib/cpu/SLFrontendC.sleela and SLFrontendCpp.sleela classes
remain limited architecture/lowering models.

SLeeLa source uses the current sleela aot --emit implementation. The command
fails closed when annotated methods exceed its supported subset, but the current
emitter still translates a whole program rather than emitting independent
method entry points. Native Drive bytecode dispatch is now separate from that
AOT limitation: syntax 3.0 exposes host-authorized `nativeOpen`, typed
`nativeCall`, and `nativeClose` built-ins.

XClass is a structural class interchange format. sleela xclass validates and
reconstructs its represented class structure into the SLeeLa AST; method bodies
that are not represented in the XClass payload are synthesized skeletons. The
Native Drive delegates XClass to this loader instead of pretending the
structural document is equivalent to C++ source.

## Native loader ABI

A shared library intended for the existing loader exports:

    uint32_t sl_native_abi_version(void);
    int sl_native_run(int argc, const char *const argv[]);

The ABI is declared in os-level-support/loader/native_loader.h. The optional
--require-sleela-abi switch checks for the required exported symbol names using
nm/dumpbin without loading the library. The loader checks the actual ABI
version at load time. Loading a native module executes code in the current
process; ABI validation is not a sandbox, signature check, or trust decision. Only load modules from a trusted location after applying the host
integrity policy.

The loader supports an optional typed `sl_native_call` entry point for
named methods. Its separately negotiated version-1 value contract covers null,
boolean, signed 64-bit integer, 64-bit double, and length-delimited UTF-8 values.
A typed plugin must export `sl_native_typed_abi_version()` and report
`SL_NATIVE_TYPED_ABI_VERSION`; a mismatch fails closed. Legacy run-only
plugins remain compatible.

The SLeeLa VM now connects this ABI to bytecode. Syntax 3.0 exposes:

- `nativeOpen(path)` — asks the host-installed authorization callback, opens
  a shared module, and returns a generation-checked VM-owned handle.
- `nativeCall(handle, method, ...args)` — dispatches through the typed ABI.
- `nativeClose(handle)` — closes and invalidates the handle.

The append-only `OP_NATIVE_CALL`, `OP_NATIVE_OPEN`, and `OP_NATIVE_CLOSE`
opcodes preserve every prior opcode number. The interpreter marshals only
null, bool, int64, double, and UTF-8 String values; it never passes its internal
`SLValue` structure, arrays, structs, closures, object handles, or pointers to
a plugin. Returned strings are copied while the module-call lock is held and
validated before entering the VM String store.

This explicit `nativeCall` route is not yet automatic dispatch for ordinary
calls to methods annotated `@native`. The annotation currently requests AOT
handling, while the AOT emitter still produces whole-program C output rather
than independently addressable method entry points. Automatic dispatch needs
compiler-emitted method identities, a per-method ABI, and an explicit host-owned
binding table before the VM can safely select native code for an ordinary
method call. The required contract and staged implementation plan are in
[`AT-NATIVE-DISPATCH.md`](AT-NATIVE-DISPATCH.md).

### Native-to-SLeeLa callbacks

A plugin may optionally export `sl_native_bind_host(const SLNativeHostAPI*)`.
The loader supplies a version-1, size-checked host table when the authorized
module is opened and rebinds it immediately before each plugin entrypoint. A
plugin copies this table into thread-local storage and may call `host_api.invoke`
synchronously from its typed method to execute an exact, unique compiled SLeeLa
function name such as `Callback::receive`. The host must separately install
`slvm_set_native_callback_authorizer`; callback targets are denied by default
and the host policy must explicitly approve each callable name. Plugins must not retain or invoke the
table asynchronously. Thread-local storage keeps callbacks associated with
the correct VM when a shared library is used by multiple VM instances.
Callback arguments and results use only the
same null/bool/int64/double/UTF-8 ABI; raw VM pointers and `SLValue` objects
are never exposed.

The VM runs callback bytecode on a separate nested interpreter stack while
preserving the suspended caller stack as a GC root. Callbacks are accepted
only when the VM has no active worker threads; callback bytecode also cannot
spawn or join workers while the plugin call is active. Recursive native-module
operations from callback bytecode (`nativeOpen`, `nativeCall`, and
`nativeClose`) are rejected with `SL_NATIVE_REENTRANT_CALL` rather than
re-entering the module mutex. The public host-side module open/run/call/close
APIs also reject same-VM reentry from an active plugin entrypoint before
attempting to acquire that mutex. Modules
without the optional binder remain compatible but cannot call back into
SLeeLa. A callback failure is a handled plugin error, not a NOT_HANDLED
fallback signal.

If a plugin reports `SL_NATIVE_CALL_NOT_HANDLED`, or an argument is not
representable by the scalar ABI, the VM may fall back to a same-name,
same-arity SLeeLa bytecode method using the original VM-owned values. Ambiguous
unqualified class methods do not fall back. Plugin errors, stale handles,
authorization failures, and ABI mismatches are surfaced as errors and never
silently routed to bytecode.

## Process policy and annotations

annotations.schema.json defines a versioned policy metadata shape for
execution mode, capability declarations, stream routing, timeout, working
directory, environment allowlist, and output bounds. It is a contract for the
next compiler/runtime integration step, not proof that these source
annotations are already parsed or enforced.

A source annotation must never grant authority by itself. The runtime must
intersect requested capabilities with the caller's configured host policy.
Native in-process execution is trusted-only; use an isolated process for
untrusted workloads. The current CLI uses argv arrays and does not invoke a
shell implicitly.

## systemd service lifecycle

tools/sleela-native-service.py renders a hardened service unit and supports
explicit user-unit installation. System-unit installation requires root and
an explicit service account; the script never invokes sudo. Enabling and
starting are separate explicit flags. The generated unit uses restrictive
systemd settings, but service sandboxing must be reviewed against each
workload's filesystem/network needs.

Example (render only; no host changes):

    python3 tools/sleela-native-service.py --name sleela-worker \
      --executable /opt/sleela/bin/worker \
      --working-directory /var/lib/sleela-worker

Install as a user service only when intentionally requested (an existing unit is never replaced unless --replace is supplied):

    python3 tools/sleela-native-service.py --install --enable \
      --name sleela-worker --executable /opt/sleela/bin/worker \
      --working-directory /var/lib/sleela-worker

For a system unit, additionally pass --system --user sleela-worker and run
with root privileges. --start is separately required to start it immediately.
systemctl is Linux/systemd-specific; native executable builds remain
available on Windows and macOS. Replacing an existing unit saves one explicit
rollback copy (`.sleela-backup`); a second replacement is refused until that
backup is rolled back. Use the lifecycle actions to inspect or control the
installed unit:

    sleelactl inspect native-drive service --status --name sleela-worker
    sleelactl inspect native-drive service --restart --name sleela-worker
    sleelactl inspect native-drive service --stop --name sleela-worker
    sleelactl inspect native-drive service --disable --name sleela-worker
    sleelactl inspect native-drive service --rollback --name sleela-worker

Rollback restores the previous unit file and runs `systemctl daemon-reload`;
it does not automatically restart the workload.

## Seven-workstream completion ledger

| Workstream | Current result | Still required |
|---|---|---|
| 1. C/C++ frontends | Production system compiler integration, multi-unit builds | Native SLeeLa frontend adapters and SLIR conformance, not a replacement C++ compiler |
| 2. SLeeLa/XClass translation | Existing @native AOT + XClass loader delegated; syntax 3.0 native built-ins compile to bytecode | Per-method AOT emission, XClass method-body fidelity where encoded |
| 3. Native build/link/load | Executable/shared builds, legacy and versioned typed ABI validation, VM-owned authorized handle registry | Deployment signature/integrity policy and compiler-generated native method metadata |
| 4. Process piping | argv execution, stdin file, bounded capture, multi-stage pipe, timeout, exit propagation | Annotation parser/runtime policy and richer stream-routing semantics |
| 5. systemd | Hardened unit rendering, explicit install/enable/start, status/restart/stop/disable, transactional failed-install recovery, one-level replacement rollback | Generated service manifests and integration tests against a real systemd host |
| 6. VM dispatch | Appended open/call/close opcodes, scalar/String marshalling, host authorization, stale-handle checks, NOT_HANDLED and VM-composite interpreter fallback | Explicit composite object/array ownership ABI, output parity, concurrency stress tests |
| 7. Conformance | Loader fixtures, versioned typed ABI, native-to-SLeeLa callback fixture, source compile fixture, opt-in VM dispatch/fallback/callback smoke target, service recovery coverage | Run local test suites and complete compiler/VM/loader/process/systemd matrix, sanitizer and cross-ABI coverage |

## Security invariants

- A path or module reference identifies code; it does not authorize execution.
- Never load caller-controlled native code in a privileged process.
- Do not treat ABI version matching as proof of trust.
- Do not use shell strings for process argument transport.
- Enforce capability policy in the runtime, not by trusting source annotations.
- Keep system service installation, enabling, and starting explicit and separately auditable.

- Treat VM destruction as a host-coordinated lifetime boundary. Native-module
  open/run/call/close operations are serialized against one another and against
  module unload by the VM's native-module mutex, but that mutex is not a
  VM-wide lifetime reference count. The host must stop and join every external
  thread that can call any VM API before calling `slvm_free()`; no API call may
  race VM destruction. This is distinct from VM-owned worker threads, which
  `slvm_free()` joins itself.
- Do not interpret serialized native-module operations as proof that arbitrary
  concurrent VM use is supported. In particular, callers must also coordinate
  VM execution, configuration, and destruction according to the public API
  contract.

## Tests

    python3 -m unittest discover -s tools/tests -p 'test_native_drive.py' -v
    make -C impl test-native-call
    make -C impl test-native-concurrency  # Linux, macOS, and Windows/MinGW
    sh -n bin/sleelactl

The focused `test-native-concurrency` target is also opt-in and is not part
of the default test aggregate or CI. On Linux and macOS it builds a typed
plugin that blocks inside `slvm_native_module_call` until the harness writes
a release marker; on Windows/MinGW it uses named Win32 events and Win32
threads for the same handshake. A second host thread requests
`slvm_native_module_close` while that call is held. The harness asserts that
the call remains active and close has not completed during the observation
window, then releases the plugin, joins both threads, checks the typed result,
and verifies that the closed generation-checked handle is stale. The Windows
fixture is selected by the Makefile only for the Windows platform; it does
not use POSIX headers, pthreads, or `-fPIC`. This test covers native-module
call/close serialization only; it does not relax the separate host
requirement to quiesce all external VM callers before `slvm_free()`.

The focused `test-native-call` target is opt-in and is not part of the default
test aggregate. It substitutes the built fixture's exact path into the
SLeeLa source fixture, compiles that source to a persistent Core artifact,
loads the artifact through `slvm_load_file`, installs a host-owned exact-path
authorization callback, and executes the program. The source program invokes
the plugin's `twice` method, calls the SLeeLa `Callback::receive` method
synchronously from native code, exercises bytecode fallback for `fallback`,
closes the module, and returns the combined result (105). The harness then reloads
the same artifact with a deliberately mismatched module allowlist and asserts
that source-level `nativeOpen` fails closed. A third run authorizes the module
but omits callback-name authorization, proving the plugin cannot invoke
`Callback::receive` unless the host explicitly allows it. The harness also
creates fresh VMs and exercises both public host-side entrypoints:
`slvm_native_module_run` and `slvm_native_module_call`. The fixture attempts
the same callback through each entrypoint without an active source-level
`nativeCall` frame; both attempts must be rejected with the exact VM error
`native callback requires an active source-level nativeCall context`.
The run-style entrypoint reports the plugin's observed refusal through its
integer result, while the typed-call entrypoint must return
`SL_NATIVE_PLUGIN_ERROR`. Both cases close the module and free the VM.
The callback-bytecode regression also attempts `nativeOpen` and
`nativeClose` from inside an authorized callback; both must fail with the
same explicit reentrancy diagnostic before touching the module table. A
callback-policy test deliberately calls each public host lifecycle API
(open/run/call/close) from the callback authorizer while the plugin entrypoint
is active; all four must return `SL_NATIVE_REENTRANT_CALL` immediately, and
the approved callback must still complete normally.

A further reentrancy regression constructs a small VM program through the public
core bytecode-building API. Its outer `OP_NATIVE_CALL` enters the fixture,
which invokes `Callback::receive_nested(value, module)`; that callback attempts
a second `nativeCall` while callback execution is active. The VM must reject
the nested operation with
`native callback cannot recursively invoke native-module operations`. The
fixture converts the rejected callback into an integer sentinel so the harness
can assert the exact VM diagnostic without masking it with an outer plugin
error. After the callback unwinds, the harness calls the same module again and
checks the expected typed result, proving the module lock was released and the
original handle remains usable. This case is deliberately a direct bytecode/API
test rather than a compiler-produced source artifact; the existing positive
callback path remains covered by the compiler-produced artifact.

The Native Drive GitHub Actions workflow is `workflow_dispatch`-only; no
workflow is automatically started by pushes. This end-to-end target has been
added but is not claimed as run until explicitly executed.
