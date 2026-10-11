# SLeeLa Compiler Developer Interface

Max Rupplin - MEARVK LLC - 2026

## Purpose

`/lib/compiler` exposes a SLeeLa-level interface for writing and orchestrating compiler implementations, plus a defined boundary for generating C and C++ from verified SLeeLa compiler IR. Developers can implement compiler phases in `.sleela`, register language front ends, and select an explicitly configured native backend.

## Source-level API

| Type | Responsibility |
|---|---|
| `SLCompilerWorkbench` | Runnable-facing build coordinator and structured diagnostics |
| `SLCompilerBuildRequest` | Input/output/target/toolchain request with fail-closed validation |
| `SLCompilerBuildResult` | Explicit result and diagnostic envelope |
| `SLCompilerDeveloperCLI` | Terminal/IDE-facing prepare and build interface |
| `SLCompilerNativeBackend` | Selects C/C++ only when the matching toolchain is configured |
| `SLNativeSourceEmitter` | Verified-IR-to-native-source backend contract |
| `SLCompilerToolchain` | Explicit C11/C++20 toolchain configuration and native-build authorization |
| `SLLanguageCompiler` | Base class for language-specific compiler front ends |
| `SLCompilerRegistry` | Shared front-end catalog |
| `SLCompilerPipeline` | Canonical compiler stages |

## Supported design paths

### SLeeLa-runnable interface

A terminal, IDE, build file, or SLeeLa program uses `SLCompilerDeveloperCLI` or `SLCompilerWorkbench` to configure a request and retrieve diagnostics/results. The native bridge is responsible for actual file access, verified-IR emission, and toolchain invocation. These operations are not simulated by the SLeeLa wrapper.

### SLeeLa source to native C/C++

The planned compiler pipeline is:

`SLeeLa compiler source -> parser/semantics -> SLeeLa IR -> verified native emitter -> C or C++ source -> selected toolchain -> native executable/library`

Generated native code is an output artifact, not an authorization to run it. It must be compiled with an explicitly selected toolchain and run only under the user's normal OS permissions and applicable SLeeLa security controls.

## Fail-closed rules

- `C` and `CPP` are the only native source targets exposed by this API.
- A source path, output path, target, language-spec version, and toolchain must be explicit.
- Unknown or unsupported constructs must produce diagnostics; no silent semantic narrowing.
- Native build authorization defaults to disabled.
- The emitter consumes verified IR, not arbitrary source strings.
- No generated program is run automatically after compilation.
- `/impl/frontend` and root `SLEELA.syntax` remain authoritative for production SLeeLa language compatibility.

## Implementation status

These `.sleela` units establish the source-level interface and bridge contract. The `sleela_compiler_emit_verified_ir` native bridge is an integration point; it must be implemented and covered by end-to-end tests before C/C++ emission can be considered operational. The top-level `/compiler` C++20 prototype remains an experimental subset.
