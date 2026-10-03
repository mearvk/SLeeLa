# SLeeLa Source → VM Completeness Contract

The VM family is one ordered source-to-execution system, not eleven unrelated source languages.

## Canonical input

Every `.sleela` file under `/lib` is recursively discovered as source input. The compiler inventory must not depend on a hand-maintained file list.

## Ordered pipeline

1. Discover application source and every `/lib/**/*.sleela` source dependency.
2. Preserve path, digest, version and dependency identity.
3. Lex.
4. Parse.
5. Build symbols for classes, fields, functions, globals, imports and annotations.
6. Bind dependencies against `/lib`.
7. Perform semantic, capability and policy checks.
8. Lower to canonical SLeeLa IR.
9. Resolve the source-side ISA registry in `/lib/vm/InstructionSet.sleela`.
10. Emit the stable SLeeLa VM artifact.
11. Validate ABI, symbols, dependencies and instruction coverage.
12. Admit through the applicable VM control layer.
13. Execute through the authoritative `/impl` Core and platform abstractions.
14. Apply the ordered VM controls /1 through /11.
15. Report source inventory, symbols, dependencies, capabilities, ISA coverage, artifact identity and diagnostics.

## VM layers

/1 executable SLVM foundation and OS boundary.
/2 cryptographic and VM-link controls.
/3 isolation and verification.
/4 authenticated/distributed execution.
/5 reproducibility, policy, recovery and assurance.
/6 lineage, attestation, leases and migration.
/7 health, memory, checkpoint, recovery and resource management.
/8 admission, policy, capability leases, transactions, audit and supervision.
/9 filesystem abstraction, native filesystem observation and filesystem modules.
/10 verified storage execution and identity/generation revalidation.
/11 standardized filesystem-module hosting.

These layers constrain and supervise the authoritative execution substrate; they do not silently become separate languages.

## ISA completeness

`InstructionSet.sleela` is the source-side ISA registry. `impl/core/sleela_core.h` supplies the native dispatch enumeration and implementation. CI must compare the registry and native ISA in both count and order.

An opcode is complete only when it has source/IR lowering, native dispatch, and execution coverage. A name appearing in the registry alone is insufficient.

## Completeness rule

source inventory = dependency inventory = compiler coverage = ISA coverage = artifact validation = runtime dispatch coverage

Missing coverage is a build failure, not a warning.


## Native implementation status

The source-to-VM pipeline is now backed by the existing native C++ frontend in `/impl/frontend` and persistent artifact emitter in `/impl/frontend/artifact.cpp`. `/lib/compiler/Makefile` builds that authoritative compiler executable through `/impl/Makefile`; `tools/sleela-build.py compile SOURCE OUTPUT` performs the library/ISA gate before invoking `sleela compile`. The recursive `/lib/**/*.sleela` inventory is therefore a real compiler input boundary, not documentation-only metadata.
