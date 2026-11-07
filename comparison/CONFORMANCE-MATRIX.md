# Conformance Matrix
Standard SCS-MATRIX-001 | Edition 1.1  
Status: baseline and numeric/Unicode batch 002 passed on configured platforms; feature qualification remains scoped.

## 1. Legend
Native/defined = language/runtime model; Managed = managed runtime; Library/API = library/platform facility; Explicit contract = must be defined and tested; Varies = target-dependent; Separate path = requires explicit interoperability.

## 2. Capability comparison
| Capability | SLeeLa | Java | C | C++ |
|---|---|---|---|---|
| Execution | SLeeLa VM / selected target | JVM | Native | Native |
| Output | SLeeLa artifact/IR | JVM class files | Native object | Native object |
| Integer widths | Syntax 3.0 width families; verify each implementation | Fixed primitive widths | Fundamental widths vary; exact-width types optional | Fundamental widths vary; exact-width types optional |
| Boolean | SLeeLa contract | boolean | _Bool | bool |
| Floating point | VM contract | Java rules | Standard + implementation | Standard + implementation |
| Strings | SLeeLa library/runtime | String | Explicit encoding convention | Library/encoding convention |
| Bounds | VM/runtime contract | Java array checks | Raw arrays unchecked | Raw arrays unchecked |
| Reclamation | SLeeLa runtime contract | Garbage collection | Explicit/manual and automatic storage | Object lifetime/RAII and explicit allocation |
| Deterministic destruction | Verify resource contract | Not guaranteed by GC | Storage-duration rules and explicit release | Scope-based object destruction |
| Exceptions | SLeeLa contract | Built-in | No standard language exceptions | Built-in |
| Concurrency | SLeeLa runtime contract | Java Memory Model | C memory model | C++ memory model |
| Native ABI | Stable C boundary intended | JNI/native interfaces if used | Platform C ABI | Compiler/platform C++ ABI |
| Java interop | External Java path documented; embedded JVM separate | JVM itself | Explicit JNI/IPC | Explicit JNI/IPC |

## 3. Repository architecture notes
Repository documentation describes SLeeLa Syntax 3.0 as current; C ABI as preferred native boundary with C++ orchestration behind it; Java interoperability as external process delegation by default; embedded JVM as not implemented in the documented default configuration; and /lib/java as a SLeeLa source/binding inventory, not a Java implementation.

These are architecture/documentation statements, not proof that every feature or platform is qualified.

## 4. Qualification checklist
| Area | Evidence required | Current status |
|---|---|---|
| Syntax/parser | Versioned compiler tests/logs | Not assessed by this comparison suite |
| Primitive representation | Runtime tests and ABI probes | Not assessed |
| Arithmetic / basic control flow / ASCII output | Shared baseline vectors | **PASS** — Ubuntu, macOS, Windows; SLeeLa VM on Ubuntu |
| Fixed-width integer edges / UTF-8 byte and code-point counts | SCS-TEST-002 edge vectors | **PASS** — C11, C++17, Java on Ubuntu, macOS, Windows; SLeeLa VM not yet covered |
| Arrays/strings beyond ASCII | Bounds and encoding tests | Not assessed |
| Memory ownership | Runtime and native sanitizer tests | Not assessed |
| C ABI | Header consumer tests across compilers | Not assessed |
| C++ wrapper | Exception containment and ABI tests | Not assessed |
| Java process | Probe, arguments, streams, exit tests | Not assessed |
| JNI/embedded JVM | Lifecycle, references, threads, shutdown | Not assessed |
| Cross-platform C/C++/Java baseline | Ubuntu, Windows, macOS workflow artifacts | **PASS** — all three OS jobs |
| SLeeLa VM baseline | Syntax 3.0 VM execution artifact | **PASS** — build and vector on Ubuntu |
| Performance | Reproducible benchmark report | Not assessed |

## 5. Corrected CI evidence

Latest baseline run: [42331591283](https://github.com/mearvk/SLeeLa/actions/runs/42331591283), conclusion success. Numeric/Unicode batch 002: [42332366905](https://github.com/mearvk/SLeeLa/actions/runs/42332366905), conclusion success, commit 087b9c2d77d0a32cf345c9ec6289a98ea0da00dd.

| Job | Result |
|---|---|
| Shared vectors — Ubuntu | PASS |
| Shared vectors — macOS | PASS |
| Shared vectors — Windows | PASS |
| SLeeLa Syntax 3.0 VM vectors — Ubuntu | PASS |

Two additional corrected runs also completed successfully: [42331589662](https://github.com/mearvk/SLeeLa/actions/runs/42331589662) and [42331561514](https://github.com/mearvk/SLeeLa/actions/runs/42331561514).

The initial run [42331126707](https://github.com/mearvk/SLeeLa/actions/runs/42331126707) failed for two diagnosed reasons: Windows Java tool discovery under MSYS2 Python, and whole-stdout comparison that treated SLeeLa runtime diagnostics as application output. The workflow now uses native Windows Python with Java 21 and the UCRT64 compiler path; the SLeeLa validator checks the final three nonblank application-output lines. The corrections passed in the completed runs above.

See [RUNNING.PASS.JAVA.001.md](RUNNING.PASS.JAVA.001.md) for baseline evidence and [RUNNING.PASS.JAVA.002.md](RUNNING.PASS.JAVA.002.md) for the numeric/Unicode edge-vector evidence and remaining scope limits.

## 6. Result record
    test_suite:
    repository_commit:
    sleela_syntax:
    sleelvac_version:
    sleela_vm_abi:
    java_version:
    c_standard:
    cpp_standard:
    compiler_and_version:
    target_triple:
    operating_system:
    build_flags:
    test_command:
    passed:
    failed:
    skipped:
    blocked:
    result_artifact:

Never replace Not assessed with PASS without an actual retained test run. A workflow definition or queued run is not a completed result.
