# ABI and Interoperability Standards
Standard SCS-ABI-001 | Edition 1.0

## 1. Boundary hierarchy
Preferred order: (1) versioned language-neutral serialized data/protocol; (2) stable C ABI with opaque handles and fixed-width values; (3) documented Java/JNI interface when a JVM integration actually exists; (4) C++ ABI pinned to compiler, standard library, and runtime; (5) private implementation details.

## 2. SLeeLa to C
The C ABI is the preferred native cross-compiler boundary. Public functions should use C-compatible declarations, explicit calling convention/export macros, fixed-width integers, opaque handles, and versioned structures.

Illustrative pattern only; these are not claims of existing exported symbols:

    typedef struct SLContext SLContext;
    typedef struct {
        uint32_t struct_size;
        uint32_t abi_major;
        uint32_t abi_minor;
        int32_t status;
    } SLResult;

    int32_t sl_context_create(SLContext **out_context);
    int32_t sl_context_run(SLContext *context, SLResult *out_result);
    void sl_context_destroy(SLContext *context);

Define structure size/version, reserved fields, nullability, ranges, ownership, release functions, thread-safety, reentrancy, errors, and diagnostic retrieval. Never expose compiler-private C++ objects through a stable C header.

## 3. C++ implementation
C++ may implement the engine, object model, orchestration, and wrappers. Keep exceptions, RTTI, templates, STL layout, and compiler-specific class layout behind the C ABI unless a particular C++ ABI is explicitly pinned. Use RAII internally and translate exceptions to stable status codes at exported C functions.

## 4. Java modes
| Mode | Description |
|---|---|
| External process | SLeeLa launches Java executable; arguments/streams/files cross process boundary |
| JNI | Native code calls into/from a JVM through JNI |
| Embedded JVM | SLeeLa process creates/manages a JVM through Invocation API |
| Serialized service | Java and SLeeLa communicate using a defined protocol |

The documented default SLeeLa Java path is external process delegation. Do not label it JNI or embedded JVM. Embedded execution requires lifecycle, thread attachment, shutdown, class-loader, exception, and platform qualification.

For process mode, pass arguments separately, avoid shell-string construction, capture exit status, and define stdin/stdout/stderr. Do not promise shared Java object identity or heap access.

## 5. Java conversions
Define primitive conversions, string encoding, array copying/pinning/lifetime, object references as opaque handles, and exception-to-status translation. JNI local/global reference and thread-attachment rules apply only when JNI is implemented.

## 6. Platform ABI
Record architecture, OS, object format, compiler, calling convention, data model, alignment, and runtime libraries. Windows x64 and System V x86-64 do not share identical calling conventions. Do not assume long width or structure packing is universal.

## 7. Versioning
Version language syntax, compiler, VM artifact format, VM ABI, native library ABI, and Java/JDK compatibility independently. Major ABI changes require explicit compatibility decisions. Minor additions must preserve layouts or use size/version negotiation.

## 8. Security
Validate lengths, indexes, tags, handles, and versions. Treat foreign inputs as untrusted. Enforce resolver/capability policy before execution. Bound memory, CPU, recursion, and message sizes. Do not interpolate untrusted input into shell commands or expose uninitialized memory.

## 9. Required tests
Test ABI mismatch, structure-size mismatch, null pointers, invalid handles, integer boundaries, string encoding, array lengths, ownership transfer, double release, errors/exceptions, process exit codes, argument boundaries, concurrency, and shutdown on each claimed platform.
