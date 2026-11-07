# Conformance Matrix
Standard SCS-MATRIX-001 | Edition 1.0  
Status: baseline planning matrix, not a test-results report.

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
| Area | Evidence required | Initial status |
|---|---|---|
| Syntax/parser | Versioned compiler tests/logs | Not assessed |
| Primitive representation | Runtime tests and ABI probes | Not assessed |
| Arithmetic | Shared test vectors | Not assessed |
| Arrays/strings | Bounds and encoding tests | Not assessed |
| Memory ownership | Runtime and native sanitizer tests | Not assessed |
| C ABI | Header consumer tests across compilers | Not assessed |
| C++ wrapper | Exception containment and ABI tests | Not assessed |
| Java process | Probe, arguments, streams, exit tests | Not assessed |
| JNI/embedded JVM | Lifecycle, references, threads, shutdown | Not assessed |
| Cross-platform | Linux, Windows, macOS logs | Not assessed |
| Performance | Reproducible benchmark report | Not assessed |

## 5. Result record
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

Never replace Not assessed with PASS without an actual retained test run.
