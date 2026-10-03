# Turing 5 Implementation Plan

The implementation is layered so the scripting runtime does not become an uncontrolled second parser inside SLeeLa.

## Phase A — Core interpreter

C/C++ implementation:

- lexer;
- precedence parser;
- runtime values;
- lexical environments;
- functions and closures;
- loops and recursion;
- structured errors;
- timeout and cancellation checks.

## Phase B — Scientific runtime

Optimized native operations for arbitrary-precision integers, real/complex arithmetic, matrices/vectors, units, calculus, numerical methods and scientific constants/definitions.

## Phase C — SLeeLa bridge

Typed host bridge for object snapshots, known-variable registry, writable-property registry, VM conditions, master sequence IDs/indexes, typed queues, pause/yield/resume and structured result return.

## Phase D — Lifecycle

Every context receives a context ID, parent document ID, VM target, capability set, deadline, resource limits, cancellation token and result channel.

## Phase E — Verification

Conformance suites cover recursion, computation, numerical accuracy, dimensional correctness, state contracts, queue schemas, timeout, cancellation, VM condition delivery and source-authority boundaries.

The existing Makefile remains the specification check until the native interpreter is implemented. Documentation must not claim an interpreter feature is executable before native code and tests exist.


## Constants implementation

Phase B scientific runtime work includes a fast indexed loader for the Demesresmes™ constants registry. The loader should validate the registry schema, preserve decimal strings until the selected numeric domain is known, attach units and uncertainty metadata, and expose read-only canonical lookup to the interpreter.

The conformance suite should verify JSON/XML semantic equivalence, exactness flags, namespace collision handling, provenance preservation, and deterministic registry revision selection.
