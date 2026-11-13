# Backup: before VM faults inside a try and division by -1 (2026-11-13)

Originals from `BASE-COMMIT.txt`, taken before SLEELA.syntax 37.5:

- `sleela_core.c`, `sleela_core.h` -- `OP_DIV` / `OP_MOD` special-case a `-1`
  divisor (no SIGFPE); `TFAULT` throws a fault (divide or modulo by zero,
  array index, checked cast) when a try handler is active, through the fault
  function (`slvm_set_fault_function`) or as the message string.
- `sleela_artifact.c` -- artifact format 6 stores the fault function index.
- `compiler.cpp` -- emits `__sleela_fault(kind, message)` when
  `SLJavaFaults.make` is loaded and registers it.
- `driver.cpp` -- loads `lib/java/support/SLJavaFaults` for a Java-style
  syntax 3.0+ document that contains `try`.

Restore: copy each file back to `impl/core/` or `impl/frontend/`, regenerate
`security/sha256-manifest.json` and rebuild.
Verify the copies: `cd impl/backup/vm-faults-2026-11-13 && sha256sum -c SHA256SUMS`.
