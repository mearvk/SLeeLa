# SLeeLa Java Interop: Compiler and VM Lowering Contract

Status: design contract for the next compiler/VM integration milestone. SLJIP v1
and the Java-side endpoint exist, but this document does not claim that source
expressions or bytecode can yet invoke them.

## Why this boundary is explicit

The current SLeeLa compiler must not infer an executable Java call from source
metadata such as java.invoke: or java.construct:. Those strings are descriptive
envelope vocabulary. Executable interop requires a semantic node, a verified
call signature, a VM operation, a native provider, and an end-to-end test.

## Semantic operation model

A future Java interop expression must resolve to one of these operations:

- resolve a class without initialization;
- construct an allow-listed class;
- invoke a public instance method;
- invoke a public static method;
- read/write a public instance field;
- read/write a public static field;
- release an opaque Java object handle.

Each operation must carry a resolved Java binary class name, member name where
applicable, ordered parameter descriptors, result descriptor, source location,
and the policy identity used for authorization. Unresolved names must fail
during semantic analysis rather than being deferred as arbitrary strings.

## Lowering requirements

The compiler/VM implementation should introduce a typed host-call representation
with the following invariants:

1. Java owner and member are compile-time validated identifiers.
2. Parameter and return descriptors are explicit; overload selection is
   deterministic and does not depend on reflective first-match ordering.
3. Only supported scalar/string/handle values can cross the current SLJIP v1
   boundary. Arrays, arbitrary collections, callbacks, and SLeeLa object graphs
   require later protocol versions.
4. The generated VM operation uses the native provider; it never constructs a
   shell command or serializes native object memory.
5. A handle is tied to one host session and is released deterministically.
6. A structured Java exception becomes a language-level error value with
   provenance; it must not become false, zero, null, or an unhandled C++ unwind.
7. Authorization is checked by the runtime as well as the compiler. A compiled
   artifact cannot widen the process allow-list.

## Proposed VM call ABI

The native provider should expose a narrow request API to the VM:

- request operation and request ID;
- validated class/member names;
- descriptor list;
- tagged scalar/UTF-8/handle arguments;
- a tagged success result or structured error result.

The provider should serialize with the SLJIP v1 codec, own one serialized
request stream per host session, and reject mismatched request IDs/opcodes.
Process startup, stdin/stdout pipe ownership, stderr draining, timeout,
cancellation, and child shutdown belong to the provider layer—not the protocol
parser or compiler.

## Required implementation order

1. Add a compiler AST node and semantic validation without emitting executable
   bytecode.
2. Add overload resolution and type-conversion tests.
3. Add a VM opcode or service dispatch entry with a versioned artifact contract.
4. Connect the VM service to the native callback exchange and a managed Java
   child process.
5. Add handle ownership to VM values and deterministic teardown.
6. Map Java exceptions to the existing SLeeLa error mechanism.
7. Add source-to-runtime integration tests for Linux, Windows, and macOS.

No syntax spelling, opcode number, or source-level usage example is prescribed
here. Those choices must follow the actual parser, AST, artifact ABI, and VM
dispatch conventions after those subsystems are inspected and changed together.
