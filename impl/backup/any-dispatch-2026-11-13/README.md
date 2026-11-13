# Backup: before interface-aware casts and calls on `Any` (2026-11-13)

Originals from `BASE-COMMIT.txt`, taken before SLEELA.syntax 43.7:

- `sleela_core.c`, `sleela_core.h` -- the VM gains a per-type supertype list
  (`slvm_add_struct_super`) so `OP_CHECKCAST`, `OP_INSTANCEOF` and typed
  `catch` see implemented interfaces, not only the `extends` chain.
- `compiler.cpp` -- registers the interface edges; emits the runtime dispatch
  chain for a method call on an `Any` receiver.
- `semantic.cpp` -- accepts a method call on an `Any` receiver when a loaded
  class declares an instance method of that name and arity.
- `java_references.cpp`, `java_references.h` -- `javaref::instanceMethodOwners`.

Later items of the same session (VM faults, 43.8) add their own files here.

Restore: copy each file back to `impl/core/` or `impl/frontend/`, regenerate
`security/sha256-manifest.json` and rebuild.
Verify the copies: `cd impl/backup/any-dispatch-2026-11-13 && sha256sum -c SHA256SUMS`.
