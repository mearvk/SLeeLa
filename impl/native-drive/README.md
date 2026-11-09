# SLeeLa Native Drive

The Native Drive is the explicit host-toolchain and process boundary for native
source execution. It connects existing SLeeLa facilities without claiming that
the model frontends under lib/cpu are production C/C++ parsers.

## Implemented

tools/sleela-native-drive.py provides:

- Direct C and C++ compilation through the configured host compiler.
- Multi-translation-unit C/C++ builds (all units in one invocation use one language).
- Executable or shared-library output.
- Optional shared-library ABI validation (sl_native_abi_version() == 1 and
  sl_native_run present) before an artifact is published.
- SLeeLa @native AOT input through the existing sleela aot --emit contract,
  followed by a host C compiler. The current emitter covers the whole program
  when its C backend can translate it; it is not yet a method-only ABI emitter.
- Argument-vector process execution without an implicit shell.
- Optional stdin-file input, stdout/stderr capture, bounded two-process piping,
  timeouts, and exit-code propagation.
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
    python3 tools/sleela-native-drive.py run --capture --timeout 10 -- build/app arg1
    python3 tools/sleela-native-drive.py pipe --timeout 10 -- producer arg :: consumer arg

## What the frontends mean

C and C++ are parsed and compiled by the selected system compiler (Clang, GCC,
or a compatible platform toolchain). This is a real production frontend path
for those languages, but it does not lower their full language grammars into
SLeeLa SLIR. The lib/cpu/SLFrontendC.sleela and SLFrontendCpp.sleela classes
remain limited architecture/lowering models.

SLeeLa source uses the current sleela aot --emit implementation. The command
fails closed when annotated methods exceed its supported subset, but the current
emitter still translates a whole program rather than emitting independent
method entry points. A full per-method ABI and transparent VM fallback remain
compiler/runtime work.

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
--require-sleela-abi switch checks the version and required symbols before
publishing the shared library. Loading a native module executes code in the
current process; ABI validation is not a sandbox, signature check, or trust
decision. Only load modules from a trusted location after applying the host
integrity policy.

The Native Drive can build a conforming library, but it does not yet register
SLeeLa source methods with the loader or dispatch VM bytecode into plugin
symbols. The current sl_native_run ABI is a module entry point, not a typed
per-method calling convention.

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

Install as a user service only when intentionally requested:

    python3 tools/sleela-native-service.py --install --enable \
      --name sleela-worker --executable /opt/sleela/bin/worker \
      --working-directory /var/lib/sleela-worker

For a system unit, additionally pass --system --user sleela-worker and run
with root privileges. --start is separately required to start it immediately.
systemctl is Linux/systemd-specific; native executable builds remain
available on Windows and macOS.

## Seven-workstream completion ledger

| Workstream | Current result | Still required |
|---|---|---|
| 1. C/C++ frontends | Production system compiler integration, multi-unit builds | Native SLeeLa frontend adapters and SLIR conformance, not a replacement C++ compiler |
| 2. SLeeLa/XClass translation | Existing @native AOT + XClass loader delegated | Per-method native emission, typed entry ABI, XClass method-body fidelity where encoded |
| 3. Native build/link/load | Executable/shared builds and optional ABI validation | Runtime-owned plugin registry, signature/integrity policy, VM-visible typed symbols |
| 4. Process piping | argv execution, stdin file, capture, two-process pipe, timeout | Annotation parser/runtime policy and full multi-stage bounded pipeline API |
| 5. systemd | Hardened unit rendering and explicit install/enable/start | Service state/rollback integration in sleelactl, generated service manifests |
| 6. VM dispatch | Stable native-loader ABI is available | VM-owned module handles, typed argument/result marshalling, fallback and parity tests |
| 7. Conformance | Native Drive smoke/integration CI | Full compiler/VM/loader/process/systemd matrix, sanitizer and cross-ABI coverage |

## Security invariants

- A path or module reference identifies code; it does not authorize execution.
- Never load caller-controlled native code in a privileged process.
- Do not treat ABI version matching as proof of trust.
- Do not use shell strings for process argument transport.
- Enforce capability policy in the runtime, not by trusting source annotations.
- Keep system service installation, enabling, and starting explicit and separately auditable.

## Tests

    python3 -m unittest discover -s tools/tests -p 'test_native_drive.py' -v
    sh -n bin/sleelactl

The GitHub Actions workflow runs the Python tests on Ubuntu, macOS, and Windows.
