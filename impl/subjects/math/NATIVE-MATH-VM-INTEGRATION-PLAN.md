# Native Math VM Integration — Completion Plan

## Integration objective

Complete the original native-math integration rather than replacing it with a
new or reduced implementation. The deliverable is one coherent path from
SLeeLa source through compiler lowering and VM dispatch to the existing native
math C ABIs, with deterministic results and explicit failure behavior.

The original scalar math integration is now restored on current master:
- The scalar IEEE-754 kernel and stable function-ID table are preserved.
- `OP_MATH_CALL` is appended at 195, with compiler lowering and native VM
  dispatch adapted from the historical implementation.
- The original SLeeLa bytecode fallback remains available when the native
  kernel declines a request.
- The historical branch's opcode numbering and stale enum were not imported.

The typed array/linear-algebra integration is also required:
- Use the existing `slmath_vm_dispatch` C ABI and `slmath_buffer` descriptors.
- Support vector, matrix, complex, decomposition, and transform operations in
  that canonical family order.
- Do not create a second VM array representation or expose VM pointers to a
  kernel as long-lived storage.

## ISA and compatibility constraints

The executable source/native ISA now ends at opcode 195. Scalar native math dispatch is implemented at 195; typed-array dispatch and later family opcodes remain reserved. Existing
assignments 0–194 are frozen and append-only, including the fixed IPC block:

- 179 — `OP_IPC_OPEN`
- 180 — `OP_IPC_CALL`
- 181 — `OP_IPC_CLOSE`

Opcodes 182–194 remain unchanged. The reserved native-math allocation is:

| Range | Family |
|---:|---|
| 195–199 | Shared native math |
| 200–207 | Vector |
| 208–215 | Matrix |
| 216–223 | Complex |
| 224–231 | Decomposition |
| 232–281 | Transform |

Within the shared block, 195 is implemented as scalar `OP_MATH_CALL`, 196 is
reserved for typed `OP_MATH_ARRAY_CALL`, and 197–199 remain vacant. All ranges are
inclusive; unused positions remain reserved for their family. Transform has
50 positions, 232 through 281.

Except for implemented opcode 195, these are reservations, not executable
instructions. Do not insert placeholder entries into the source registry or
native enum. Reserved IDs must fail closed until implementation, compiler
lowering, dispatch, artifact validation, supported VM-generation contracts,
tests, and documentation land together.
Function IDs and typed operation IDs remain distinct from opcode IDs. The
historical native-math branch's assignment of `OP_MATH_CALL` to 179 is invalid
for this repository state.

## VM array ownership and safety contract

VM arrays remain `SL_ARRAY` values containing VM-local handles into the
existing bounded `SLArrayInstance` table. The VM owns allocation, GC
reachability, element tags, and lifetime.

Before a native array operation:

1. Validate each operand is an active VM array handle.
2. Validate every element is a supported numeric tag (`SL_INT`,
   `SL_INT128`, or `SL_DOUBLE`); reject mixed/non-numeric data unless the
   specific operation explicitly defines a conversion.
3. Validate dimensions, multiplication overflow, required lengths, and output
   capacity before allocating scratch buffers or entering a kernel.
4. Keep all participating array values rooted on the executing VM stack until
   dispatch and write-back finish. Do not pop the roots early.
5. Serialize access to participating arrays for the complete snapshot/kernel/
   write-back transaction, or use an equivalent pin/version protocol that
   prevents concurrent mutation and reclamation.
6. Marshal into bridge-owned scratch buffers. Never give the kernel a pointer
   into a resizable `SLArrayInstance::elems` allocation.
7. Copy results back only after a successful kernel status. On failure, leave
   output arrays unchanged.
8. Release locks and scratch allocations on every success and error path.

Native kernels are synchronous and must not retain descriptor pointers after
return. The VM remains the sole owner of all source and destination arrays.
Aliasing rules are operation-specific and must be validated before dispatch.

## Typed buffer and shape contract

The existing `slmath_buffer` descriptor carries:
- `data`: bridge-owned `double` storage for the duration of dispatch;
- `length`: allocated element count;
- `rows` and `cols`: explicit logical shape.

Shape products must be checked for `size_t` overflow. The VM adapter must
validate both descriptor capacity and the operation's shape constraints before
calling `slmath_vm_dispatch`. Dimensions must be explicit at the SLeeLa
call/lowering boundary; they must not be guessed from a flat array's length.

## Kernel dispatch and operation families

Keep the scalar C ABI (`sl_math_kernel_*`) and typed structure C ABI
(`slmath_vm_dispatch`) as distinct, versionable interfaces behind one VM-owned
dispatch boundary. The scalar function-ID table remains stable and append-only.
Typed operation IDs remain those declared by `linear_algebra.h`.

Canonical extension order:
1. Vector
2. Matrix
3. Complex
4. Decomposition
5. Transform

Do not allocate extra opcodes for individual math functions unless a reviewed
implementation proves one generic dispatch instruction cannot express the
required contract.

## Compiler and artifact contract

Compiler lowering must be deterministic:
- Resolve supported scalar names to stable scalar function IDs.
- Resolve typed operations to stable operation IDs.
- Emit all operands and dimensions in the documented stack order.
- Reject unsupported arity, type, or shape combinations at compile time where
  statically knowable; retain runtime checks for dynamic values.
- Never silently substitute a different native operation.
- Preserve scalar bytecode fallback only for the scalar ABI's documented
  decline case; typed-array validation/kernel errors are explicit failures.

Artifact validation must reject unknown operation IDs, invalid operands,
impossible argument counts, and malformed stack contracts before execution.

## Required regression coverage

### Array ownership and marshalling
- Invalid, stale, and wrong-type handles.
- Empty arrays, unsupported element tags, mixed numeric tags, and conversion
  boundaries.
- Dimension/length mismatch and integer overflow in shape products.
- Output capacity failures and allocation failures.
- Input/output aliasing, including exact aliases where supported.
- GC pressure while arrays are rooted; verify no use-after-free or stale write.
- Concurrent readers/writers; verify transaction semantics and no partial
  write-back.
- Kernel failure leaves destination arrays unchanged.

### Scalar integration
- Every scalar function ID, exact arity, constants, and invalid IDs.
- Numeric conversion boundaries and non-finite IEEE-754 behavior.
- Native success and bytecode fallback when the native kernel declines.
- Compiler lowering and artifact round-trip preserve function IDs.

### Typed operation integration
- Positive and negative cases for each vector, matrix, complex,
  decomposition, and transform operation.
- Numerical tolerances, dimensions, aliasing, singularity, and convergence
  behavior.
- Cross-platform C ABI layout and symbol checks.

## Delivery sequence

1. Recover and reconcile the original scalar integration against current master — complete: opcode 195, compiler lowering, VM dispatch, C ABI linkage, and artifact operand validation are now integrated.
2. Implement the VM-owned array validation, rooting, and transaction adapter.
3. Connect the existing typed C ABI through that adapter.
4. Add the append-only opcode/source ISA/native enum/artifact validation as one
   coordinated change.
5. Add deterministic compiler lowering and explicit diagnostics.
6. Add regression coverage for ownership, marshalling, dispatch, and numerical
   behavior.
7. Update the opcode map, VM-generation contracts, API docs, and trusted
   manifests; then run targeted qualification followed by the broader build
   matrix.

Do not describe standalone C ABI qualification as proof of VM bytecode
integration. Do not mark this integration complete until the source compiler,
artifact loader, VM dispatch, tests, and documentation all agree.
