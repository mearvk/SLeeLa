# Native Math VM Array Adapter — Source Audit

Date: 2026-11-15
Branch reviewed: `master`
Status: source audit complete; VM adapter and opcode 196 remain unimplemented.

## Executive result

The scalar path at opcode 195 and the standalone typed linear-algebra C ABI are
present. The VM-owned array adapter is not yet present. It is not safe to
activate opcode 196 by merely constructing `slmath_buffer` descriptors around
the existing VM array storage.

The review deliberately leaves opcode 196 unallocated in the executable
source/native ISA. No existing opcode was changed.

## Verified source facts

1. `SLArrayInstance` stores `active`, `len`, `cap`, element-reference
   metadata, an `SLValue *elems` allocation, and a GC object pointer.
2. `array_valid_handle` validates a handle by checking the slot range and
   `active` flag. The array instance shown in the current Core source has no
   generation/incarnation field.
3. Array allocation uses a bounded reusable slot table. Therefore an integer
   slot handle does not by itself distinguish an old occupant from a later
   occupant of the same slot. The existing array-handle lifetime model needs
   hardening before the adapter can claim stale-handle rejection.
4. The interpreter array fast path may skip `array_mtx` when the executing
   thread's cached worker count is zero. This is an interpreter optimization,
   not a general native-bridge synchronization guarantee.
5. `OP_NEWARRAYND` builds nested arrays. It does not create a flat,
   contiguous numeric matrix with explicit row/column metadata.
6. The existing `slmath_buffer` descriptor contains a `double *`, element
   length, rows, and columns. `slmath_vm_dispatch` is a synchronous typed
   numerical ABI; it does not own or validate SLeeLa VM array handles.
7. The scalar `OP_MATH_CALL` path is separate and has an established
   bytecode fallback. Typed-array failures must not be routed through that
   fallback.

## Required implementation order

### A. Harden VM array identity

Before a VM math adapter is introduced, give array handles an incarnation-safe
identity so a stale value cannot alias a reused slot. Review all array creation,
lookup, GC reclamation, array opcodes, artifact/value serialization boundaries,
and any host-facing array APIs together. Preserve existing source-level array
semantics and reject invalid generations explicitly.

This is a Core array-lifetime change, not a math-kernel concern. Do not bolt a
generation check onto only opcode 196 while leaving `OP_ARRGET`, `OP_ARRSET`,
`OP_ARRLEN`, `OP_ARRPUSH`, and other array consumers on slot-only handles.

### B. Define a real typed-array shape boundary

Specify how source code supplies or constructs a validated shape descriptor.
Do not infer a matrix shape from a flat array length and do not reinterpret
nested arrays. The descriptor must carry the intended element count and
dimensions, and the compiler and artifact validator must agree on its layout.

### C. Implement transactional marshalling in shared Core

Keep operands rooted, validate generation/slot/length/capacity/tags, stage
bridge-owned input and output buffers, validate all size arithmetic, call
`slmath_vm_dispatch`, and commit only after success. Audit the array lock
policy and GC interactions. No kernel receives an `SLValue *` or pointer into
a resizable VM-owned element buffer.

### D. Activate the opcode only as one coordinated change

Only after A–C, add `OP_MATH_ARRAY_CALL` at 196, deterministic compiler
lowering, artifact validation, fail-closed dispatch, source/native ISA checks,
rollback and concurrency tests, and SLVM/1–11 shared-Core qualification.
Preserve the family allocation: 195–199 shared math, 200–207 vector,
208–215 matrix, 216–223 complex, 224–231 decomposition, 232–281 transform.

## Qualification still required

- Array slot reuse and stale-generation rejection across all array opcodes.
- Invalid handles, wrong tags, nested arrays, empty arrays, and mixed numeric
  values.
- Shape mismatch and `size_t` multiplication/byte-size overflow.
- Output-capacity and allocation-failure behavior.
- Exact alias rules and rejection of ambiguous multi-output aliases.
- GC pressure and concurrent worker mutation during snapshot/dispatch/commit.
- Every kernel failure leaves destination arrays unchanged.
- Numerical positive/negative tests for each supported typed operation.
- Artifact validation, compiler round-trip, ISA equality, and all eleven
  shared-Core VM runners.
- Linux, Windows, and macOS build compatibility.

## Current disposition

Opcode 195 remains the scalar implementation. Opcode 196 and the remaining
math-family positions remain reservations. The standalone linear-algebra
smoke test is evidence for the C ABI only; it is not evidence of VM-array
integration. No typed VM execution or cross-platform qualification is claimed
by this audit.
