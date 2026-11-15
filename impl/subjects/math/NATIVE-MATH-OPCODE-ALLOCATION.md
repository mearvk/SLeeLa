# Native math opcode allocation and VM integration plan

## Current ISA boundary

The Native IPC worker opcodes remain one contiguous, append-only group:

- 179 — `OP_IPC_OPEN`
- 180 — `OP_IPC_CALL`
- 181 — `OP_IPC_CLOSE`

The current source ISA continues after that group: `OP_REF` occupies 182 and
subsequent instructions occupy 183–194. The old native-math branches assigned
`OP_MATH_CALL` to 179; that assignment is incompatible and must never be
copied into the current tree.

## What is integrated now

The standalone native scalar math C ABI is present in `math_kernel.hpp`,
`sleela_math_kernel_c.h/.cpp`, with an opt-in
`make -C impl test-native-math-kernel` qualification target. It covers the
IEEE-754 scalar function set and stable function-ID/arity/method-name dispatch.

The native linear-algebra C ABI is present in `linear_algebra.h/.cpp`, with an
opt-in `make -C impl test-linear-algebra` qualification target. It covers
vectors, matrices, LU/QR, complex arithmetic, eigen decomposition, FFT, and
typed buffer dispatch.

This is a native library boundary only. It does not yet pin VM-owned arrays,
validate VM object layouts, marshal VM arrays into the typed descriptors, or
execute a math opcode from SLeeLa bytecode. The standalone C ABI test must not
be described as VM bytecode integration.

## Required integration continuity

The VM work must complete the original scalar native-math integration as well
as the typed array/linear-algebra path. The scalar function-ID table,
`sl_math_kernel_*` C ABI, compiler lowering, native dispatch, and documented
bytecode fallback are not to be dropped or replaced by an array-only design.
The historical branch is a source of implementation behavior, not an ISA
source: its `OP_MATH_CALL = 179` assignment conflicts with the fixed IPC block
and must not be imported.

The full ownership, marshalling, compiler, artifact, regression, and delivery
contracts are maintained in
[`NATIVE-MATH-VM-INTEGRATION-PLAN.md`](NATIVE-MATH-VM-INTEGRATION-PLAN.md).

## Append-only rule for future VM dispatch

The next available numeric ID after the current source ISA is 195.
To preserve the original scalar integration, the intended order is:

- 195 — `OP_MATH_CALL`: scalar IEEE-754 dispatch using the stable
  `SLMATHFunction` ID table and the original fallback stack contract
  `[fallback marker, helper-name String, numeric arguments...]`.
- 196 — `OP_MATH_ARRAY_CALL`: typed VM-array dispatch through
  `slmath_vm_dispatch`, after the VM ownership, validation, and marshalling
  adapter is implemented.

This deliberately separates the established scalar ABI from typed array
operations while keeping both in one contiguous native-math opcode block. The
old branch's scalar implementation is the compatibility reference for 195;
its obsolete assignment of 179 is not. Do not insert math IDs between 179–181,
and do not renumber 182–194.

If additional math-specific opcodes are later proven necessary, append them
after 196 in the canonical operation family order: vector, matrix, complex,
decomposition, transform. Prefer operation selectors within the typed array
dispatch over one opcode per mathematical function.

No opcode ID is reserved by this planning document. Allocation becomes
authoritative only when the same change includes, in maintenance order:

1. `lib/vm/InstructionSet.sleela`;
2. native `SLOp` enum and compile-time ID assertions;
3. VM dispatch with bounds, ownership, pinning, and error handling;
4. `lib/vm/OPCODE-MAP.md`;
5. source/native ISA equality and artifact validation;
6. VM-generation contracts, tests, and trusted-manifest updates.

Until then, the C ABI remains independently usable and the bytecode path
remains explicitly unimplemented.
