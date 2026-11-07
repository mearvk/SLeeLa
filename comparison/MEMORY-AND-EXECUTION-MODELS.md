# Memory and Execution Models
Standard SCS-MEM-001 | Edition 1.0

## 1. Execution representations
| Environment | Representation | Engine |
|---|---|---|
| SLeeLa | source and compiled VM artifact/IR | SLeeLa VM or explicitly selected target |
| Java | class files / JVM bytecode | JVM |
| C | native object/executable | processor and OS |
| C++ | native object/executable | processor and OS |

A SLeeLa native implementation written in C/C++ still executes SLeeLa semantics when it runs SLeeLa programs through the VM.

## 2. Allocation and lifetime
- SLeeLa: document VM allocation, heap/stack, references, collector behavior, resource limits, and release facilities from the actual implementation.
- Java: objects are garbage-collected; reclamation timing is not deterministic.
- C: explicit allocation/release such as malloc/free plus automatic storage duration.
- C++: object lifetime rules, scope-based destruction and RAII, plus dynamic allocation and owning library types.

GC does not replace explicit closure of files, sockets, and locks. C++ destruction is not automatically safe if ownership or pointer lifetime is wrong.

## 3. Ownership modes
Every native boundary value must be classified as borrowed for call duration, borrowed until explicit release, transferred, shared under a documented handle/refcount protocol, or copied. State who releases it, which allocator is used, whether retention is allowed, and failure cleanup.

Do not free memory in another runtime/allocator domain unless compatibility is guaranteed. Prefer paired create/destroy functions owned by the same module.

## 4. References and handles
Raw pointers are process-local details. Never serialize them or treat them as durable identifiers or execution authority. Cross-runtime references should use opaque handles with validity/generation checks and explicit release. Java object references are not portable C pointers; SLeeLa references must use the documented VM boundary.

## 5. Garbage collection
Java GC timing is not an application resource-management contract. Finalization is deprecated. If SLeeLa uses a collector, document roots, triggers, stack/register assumptions, pause behavior, and heap limits. Do not claim equivalence to Java GC without evidence.

C has no standard collector. C++ does not require one; deterministic object lifetime and RAII are common resource-management techniques.

## 6. Stack and resource limits
Record stack, recursion, heap, array, CPU, and message limits and failure behavior. Distinguish stack overflow, VM limit failure, process termination, and successful completion. Never silently change a requested computation or allocation.

## 7. Errors and exceptions
Java has checked and unchecked exceptions. C has no standard language exception mechanism and uses explicit error conventions. C++ supports exceptions, but they must not cross a C ABI boundary. SLeeLa behavior follows its language/VM contract.

Translate errors at interop boundaries into stable status/result structures. Catch and translate C++ exceptions before returning through C.

## 8. Concurrency
- Java: Java Memory Model.
- C: C memory model and atomics.
- C++: C++ memory model and atomics.
- SLeeLa: its VM/runtime threading and synchronization contract.

Compare only defined synchronized behavior. Use locks, atomics, or message passing; do not use a data race as a portable expected result.

## 9. Determinism
State whether result order, scheduling, reclamation, floating-point behavior, and timing are deterministic. C++ scope destruction, Java GC, and a VM collector are not interchangeable guarantees.

## 10. Required tests
Test allocation failure, bounds, use-after-release prevention, double release, ownership transfer, aliasing, cleanup on error, thread visibility, and shutdown. Run native sanitizers where available and record skipped tools.
