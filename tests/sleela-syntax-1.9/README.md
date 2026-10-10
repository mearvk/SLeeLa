# SLeeLa syntax 1.9 regression fixtures

These fixtures exercise the syntax 1.9 feature: the **GC-hint statement**
`x = gc N;` / `x = mem N;`, a developer-emitted request for a short-term
garbage-collection cleanup of unreachable allocations after a value is done
being used. `N` is a 0..100 aggressiveness (0 is a no-op; higher runs a larger
young-only safepoint collection; 100 requests a full collection). The bare form
`x = gc;` defaults to a modest cleanup. Both the `gc` and `mem` spellings are
accepted and are **contextual** — `gc`/`mem` remain ordinary identifiers
everywhere except as the whole right-hand side of an assignment.

Passing:

- `gc-hint-pass.sleela` must build, run, and print the unaffected values; the
  hints (`gc 100`, `mem 25`, bare `gc`) do not alter the program's observable
  result — they only ask the VM to reclaim unreachable memory.
- `gc-hint-contextual.sleela` must build and run: `gc` used in an expression
  (`let x = gc + 5;`) is a normal variable, while `x = gc;` (whole RHS) is the
  hint; the hint leaves the target value unchanged.

Failing (must produce an explicit semantic error):

- `gc-hint-range-invalid.sleela` — an aggressiveness outside 0..100 is rejected
  (`GC hint aggressiveness must be 0..100`) rather than silently clamped.
- `gc-hint-version-invalid.sleela` — the hint under `#sleela 1.8` is rejected
  (`GC hint ... requires syntax 1.9`).

Run them from the repository root after building the native compiler:

```sh
make -C impl
impl/build/sleela run tests/sleela-syntax-1.9/gc-hint-pass.sleela
impl/build/sleela run tests/sleela-syntax-1.9/gc-hint-contextual.sleela
impl/build/sleela run tests/sleela-syntax-1.9/gc-hint-range-invalid.sleela
impl/build/sleela run tests/sleela-syntax-1.9/gc-hint-version-invalid.sleela
```

The `-invalid` commands are expected to fail with an explicit semantic error.
The GC hint reclaims only genuinely unreachable objects; it never frees a live
value, and (like every collection safepoint) it is deferred while worker threads
are active.
