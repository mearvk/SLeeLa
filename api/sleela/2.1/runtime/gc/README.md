# Runtime Garbage-Collection API

This directory describes the language-level GC hint introduced in syntax 1.9 and links it to the native collector tests. It does not replace the implementation contract in `impl/` or the tracked native allocation API in [`api/native/memory/`](../../native/memory/README.md).

- [GC-hint API](GC-HINT-API.md) — accepted forms and range.
- [Semantics](SEMANTICS.md) — safety and behavior boundaries.
- [Test matrix](TEST-MATRIX.md) — fixture-to-contract traceability.
- [Native collector integration](NATIVE-COLLECTOR.md) — root enumeration, barriers, ownership, failure behavior, and the hard-memory-limit distinction.

A GC hint is a runtime request. It must not invalidate reachable values or change program-observable results merely because collection was requested. Do not promise immediate reclamation of every unreachable object unless the runtime implementation and tests establish that behavior for the relevant collection mode.
