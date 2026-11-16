# VM-owned native math array adapter contract

Status: implementation contract; adapter and opcode 196 are not yet enabled.

## 1. ISA allocation is fixed

The executable ISA remains append-only. Keep the native-math instructions in
contiguous, ordered family blocks:

| Opcode range | Family | Current rule |
|---:|---|---|
| 195–199 | Shared native math | 195 scalar dispatch; 196 typed-array dispatch only after adapter completion; 197–199 reserved |
| 200–207 | Vector | Reserved as one contiguous family |
| 208–215 | Matrix | Reserved as one contiguous family |
| 216–223 | Complex | Reserved as one contiguous family |
| 224–231 | Decomposition | Reserved as one contiguous family |
| 232–281 | Transform | Reserved as one contiguous family |

The IPC group 179–181 is immutable and remains contiguous. Existing opcodes
0–194 retain their current values. Reservations are allocation metadata, not
executable enum entries: do not add placeholder opcodes to the native enum or
source registry. The executable registry and native enum must remain identical.

## 2. Ownership boundary

The VM remains the sole owner of arrays. The adapter works with the existing
VM-local array handles and `SLArrayInstance` storage; it must not introduce a
second VM array representation.

**Precondition discovered in the 2026-11-15 source audit:** the current array
instance has no generation/incarnation field, and `array_valid_handle` checks
slot range plus `active` only. Because the bounded table reuses slots, this is
not sufficient to distinguish a stale handle from a later occupant. Before
native array dispatch can claim stale-handle safety, array identity must be
hardened across *all* array consumers (`OP_ARRGET`, `OP_ARRSET`, `OP_ARRLEN`,
`OP_ARRPUSH`, allocation, GC reclamation, and host-facing APIs). Do not add a
private generation check only to opcode 196; that would leave inconsistent
array-handle semantics. See `VM-ARRAY-ADAPTER-SOURCE-AUDIT.md`.

The adapter must:
- validate every handle, active slot, logical length, capacity, and element;
- accept only explicitly supported numeric element tags (`SL_INT`,
  `SL_DOUBLE`, and, where loss policy is explicitly approved, `SL_INT128`);
- reject booleans, strings, structs, nested arrays, nulls, and other tags
  rather than coercing them silently;
- keep operand handles rooted on the VM operand stack for the entire call;
- never pass `SLValue*` or VM-owned element pointers to a native math kernel;
- never allow a kernel to retain a pointer after dispatch returns.

## 3. Snapshot and transaction

For each operation, the adapter stages all inputs and outputs in adapter-owned
numeric buffers. It validates multiplication/byte-size overflow before
allocation and handles allocation failure without modifying VM arrays.

The operation follows this transaction:

1. Acquire the existing `array_mtx` using the Core's established lock policy.
2. Validate all handles, shapes, lengths, element types, and alias rules.
3. Copy inputs into owned `double` buffers; initialize separate staged output
   buffers.
4. Invoke `slmath_vm_dispatch` using descriptors whose pointers refer only to
   those owned buffers.
5. Validate the returned status and every output value/length.
6. Commit staged results to the destination array(s) only on success.
7. Release the lock and free every temporary buffer on every exit path.

Holding the array lock across validation, snapshot, dispatch, and commit gives
the first implementation a simple serializable transaction with the existing
array mutation opcodes. Do not use `workers_cached == 0` to bypass this
transaction lock: that optimization is specific to interpreter-owned array
opcodes and is not a proof that an external native dispatch is race-free.
If later profiling justifies releasing the lock during kernel execution, add
per-array versioning or another explicit conflict-detection protocol first.

Exact input/output aliasing is safe only because inputs are snapshotted before
the kernel runs. Multiple output descriptors that alias one VM array must be
rejected unless a particular operation explicitly defines their commit order.
Partial overlap is not representable by independent VM array handles and must
not be synthesized.

## 4. Shape contract

The adapter must not infer matrix dimensions from flat array length alone.
Every operation that needs shape information must receive validated dimensions
from an explicit VM-level descriptor. Validate:
- non-negative dimensions;
- checked `rows * cols` and byte-size arithmetic;
- descriptor element count equals the backing array's logical length;
- operation-specific dimension constraints before dispatch.

The compiler/bytecode stack contract for this descriptor must be documented
before opcode 196 is added. Existing nested-array semantics must not be silently
reinterpreted as flat matrices.

## 5. Failure behavior

Validation errors, unsupported element types, shape mismatches, overflow,
allocation failure, alias violations, and native-kernel errors are explicit VM
faults. They do not trigger the scalar opcode 195 bytecode fallback. A failed
operation leaves every destination array byte-for-byte/value-for-value
unchanged.

The scalar fallback contract at opcode 195 remains unchanged.

## 6. Implementation and qualification gates

Opcode 196 may be appended to the executable enum only when all of the
following land together:

1. Incarnation-safe array handles across the shared Core array API, including
   slot reuse and GC reclamation.
2. A VM-owned adapter in the shared Core engine.
3. A documented stack/shape/alias contract and deterministic compiler lowering.
4. Artifact operand validation for the typed operation identifier.
5. Dispatch through the existing `slmath_vm_dispatch` C ABI.
6. Tests for stale/invalid handles, non-numeric values, empty arrays, shape
   mismatch, overflow, allocation failure, aliases, worker contention,
   rollback on every kernel failure, and numerical correctness.
7. Source/native ISA equality and opcode-allocation checks.
8. Qualification of the shared Core path used by SLVM/1 through SLVM/11.

Until then, 196–281 remain reserved and fail closed. Do not renumber, fill
unrelated gaps, or split a family across non-contiguous ranges.
