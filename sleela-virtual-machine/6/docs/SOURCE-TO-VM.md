# SLVM/6 Source-to-VM Contract

SLVM/6 participates in the common pipeline defined by `/VM.SOURCE.PIPELINE.md`.

1. Accept a validated SLeeLa VM artifact.
2. Verify source identity, artifact ABI and dependencies.
3. Verify the canonical source-defined ISA registry.
4. Apply SLVM/6 control and security requirements.
5. Delegate actual bytecode execution to the authoritative execution substrate where applicable.
6. Preserve source, artifact, capability and diagnostic identity.
7. Reject missing prerequisites instead of changing language semantics.

Every `.sleela` under `/lib` is part of the recursive source inventory. New library source therefore cannot silently become an untracked VM feature.

SLVM/6 is an ordered architectural layer, not a separate SLeeLa language.

## Opcode support and unsupported-opcode handling

This generation consumes the canonical source ISA in `/lib/vm/InstructionSet.sleela`. The current repository verification finds **98 source opcodes, 98 native enum entries, and 98 native dispatch cases**, with no missing dispatch cases.

The support map is maintained in `/lib/vm/OPCODE-MAP.md`. The authoritative execution dispatch remains `/impl/core/sleela_core.c`.

If an opcode outside the supported map reaches execution, it is **not** silently ignored. The runtime reports the numeric opcode to standard error, or appends the diagnostic to the path named by `SLEELA_OPCODE_LOG`, and rejects execution with an `unsupported opcode` error.

This is a generation-wide contract: SLVM/1 through SLVM/11 must preserve the same source-defined ISA and must not reinterpret an unsupported opcode as a valid operation.

## Runtime services: networking, files, sockets, threads, GC, and teardown

Each VM generation participates in `/lib/vm/RUNTIME-SERVICES.md`. These are construction invariants, not optional documentation:

- **Networking:** capability-mediated connect/listen/accept, deadlines, cancellation, bounded buffering/backpressure, readiness, and explicit transport errors.
- **File I/O:** opaque VM-local handles, open/read/write/close/unlink, EOF/error distinction, ownership, and recovery-safe invalidation.
- **Sockets:** explicit lifecycle from NEW through CONNECTED/HALF_CLOSED to CLOSED/FAILED; native descriptors and Windows HANDLEs remain private to the platform adapter.
- **Threading:** structured ownership of `SPAWN` children, cancellation propagation, join results, bounded execution, and safe points. Native pthread/Windows primitives are implementation adapters.
- **Garbage collection:** tracing VM heap semantics with incremental/generational collection where practical, precise roots, write barriers, and safepoints. GC does not implicitly own scarce OS resources.
- **Teardown:** idempotent ordered shutdown: quiesce, cancel, drain, join, close resources, finalize language objects, then release VM memory. Lifecycle violations fail closed/quarantine.

The existing 98-opcode ISA supplies the direct networking, socket, file, pipe, FIFO, threading, synchronization, and asynchronous primitives. GC, ownership, cancellation, deadlines, resource epochs, and teardown remain runtime services rather than hidden opcodes.

Generation-specific additions may strengthen validation or recovery, but SLVM/1 through SLVM/11 preserve these resource-lifetime and source-semantics invariants.
