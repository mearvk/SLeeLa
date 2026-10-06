# Change Write-Up: Memory Manager + Native Executable Launcher

**Commit:** `6736047` on `master`
**Scope:** Add a flag-enabled Memory Manager and native-executable terminal
launching to the SLeeLa toolchain; fix a pre-existing broken test target.

---

## Summary

Two related capabilities were added to the SLeeLa runtime, plus a build fix:

1. **Native executables run from the terminal** through the `SLeeLa` launcher.
2. **A Memory Manager** for raw process-memory accounting and hard limits,
   enabled by a flag at startup or automatically when running a native.

---

## 1. Native executable launcher

The `SLeeLa` launcher already ran `.sleela` objects; it now also runs an
arbitrary **native OS executable** from the terminal.

- New subcommand **`sleela native|exec [--] <program> [args...]`**
  (`nativeCmd` in `impl/frontend/driver.cpp`).
- Runs the program under a real pseudo-terminal via the existing OS-aware PTY
  primitive `slterminal_spawn` (POSIX `fork`+`execl`; Windows ConPTY +
  `CreateProcessW`).
- Relays the child's output to stdout and reaps it on close.
- Honors the same **SHA-256 execution gate** as `run`/`check`/`compile`.
- Every argument is shell-quoted before being handed to the PTY.
- `--` stops option parsing so the native program can take its own options.
- The launcher script `bin/SLeeLa` passes `native`/`exec` through verbatim
  (no `.sleela` object resolution) and forwards leading `--memory-manager`
  options; help text was updated.

```sh
SLeeLa native /usr/bin/env
SLeeLa exec ./my-tool --flag value
```

## 2. Memory Manager

New component `impl/core/sleela_memmgr.{h,c}`, layered on top of the OS
allocator abstraction (`impl/core/sleela_memory.c`).

- **Opt-in.** Disabled by default; when off the wrappers forward straight to
  the OS allocator with no accounting overhead.
- **Exact accounting.** Each block carries a small aligned size header, so
  `free`/`realloc` update the running total exactly with no side table. Tracks
  live bytes, peak bytes, and live/total allocation counts.
- **Hard limit, fail-closed.** An optional byte limit refuses an allocation
  (returns `NULL`, records a refusal) rather than over-committing the host.
- **Thread-safe** via the `slthread` mutex abstraction.
- **Distinct axis** from the mark-sweep GC (`runtime/garbage_collector.c`,
  managed objects) and the quota supervisor (`runtime/security_supervisor.c`,
  per-class/role budgets): this manager accounts for *raw process memory*.

### Enabling it

- CLI: leading `--memory-manager[=<size>]` (or `--mm`) before the subcommand.
- Env: `SLEELA_MEMORY_MANAGER=<size>` (`off`/`0` disables).
- `<size>` accepts a byte count or a `K`/`M`/`G` suffix (optional trailing `B`).
- **Automatic ("on-need")** for `native`/`exec`.
- A one-block stats report is printed to stderr at exit when enabled.

```sh
sleela --memory-manager run examples/hello.sleela
sleela --memory-manager=64M run program.sleela
sleela native --memory-manager=128M ./tool
```

### API (`impl/core/sleela_memmgr.h`)

`slmm_enable`/`slmm_disable`/`slmm_is_enabled`/`slmm_limit`/`slmm_set_limit`;
`slmm_alloc`/`slmm_calloc`/`slmm_realloc`/`slmm_free`;
`slmm_stats`/`slmm_last_status`/`slmm_status_name`/`slmm_reset`/`slmm_format_report`.
Status codes: `OK`, `DISABLED`, `LIMIT`, `OOM`, `OVERFLOW`.

> Pointers from `slmm_*` carry a header and must be released with `slmm_free`
> / resized with `slmm_realloc`, never raw C `free()`.

## 3. Build, tests, and a pre-existing fix

- Wired `sleela_memory.o` and `sleela_memmgr.o` into `impl/Makefile`; both are
  linked into `sleela` and `nordshrift`.
- Added `impl/tests/core/memmgr_smoke.c` (20 behavioral assertions:
  untracked-when-disabled, exact accounting across
  alloc/calloc(zeroed)/realloc(preserving)/free, and a hard limit that fails
  closed) and a `test-memmgr` target in the aggregate `make test`.
- Added `_POSIX_C_SOURCE` to `sleela_memory.c` (for `posix_memalign`), now that
  the file is compiled for the first time — resolves a warning.
- **Fixed a pre-existing breakage** (present on upstream `master`):
  `thread_smoke` and `socket_smoke` did not link the core's
  io/net/path/library/terminal backend objects that `sleela_core.o` references,
  which broke `make test` at those targets. They now link the full backend set.
- Regenerated the SHA-256 build manifest; a fresh regeneration is byte-identical
  to the committed file, so the CI manifest-sync check passes.

### Verification

`make clean && make test` exits 0. All subject numeric checks pass;
`RUNTIME SMOKE: PASS`; `MEMMGR SMOKE: PASS`. CLI verified end-to-end: the flag,
`=<size>` with K/M/G suffixes, the `SLEELA_MEMORY_MANAGER` env var, invalid-size
rejection, and `native`/`exec` running `/bin/echo`, `uname`, and `/bin/false`.

## Files changed

| File | Change |
|---|---|
| `impl/core/sleela_memmgr.h` | new — Memory Manager API |
| `impl/core/sleela_memmgr.c` | new — Memory Manager implementation |
| `impl/core/sleela_memory.c` | `_POSIX_C_SOURCE` feature-test macro |
| `impl/frontend/driver.cpp` | `--memory-manager` flag, `native`/`exec` subcommand, helpers |
| `impl/Makefile` | build/link MM objects, `test-memmgr`, thread/socket smoke link fix |
| `impl/tests/core/memmgr_smoke.c` | new — behavioral test |
| `bin/SLeeLa` | forward MM options, `native`/`exec` passthrough, help |
| `bin/README.md` | launcher docs |
| `README.md` | doc-list entry + terminal-run examples |
| `MEMORY_MANAGER.md` | new — full reference |
| `security/sha256-manifest.json` | regenerated |

## Documentation

Full reference: [`MEMORY_MANAGER.md`](MEMORY_MANAGER.md). Launcher usage:
[`bin/README.md`](bin/README.md). OS allocator layer (unchanged):
[`impl/MEMORY_SYSTEM_OS.md`](impl/MEMORY_SYSTEM_OS.md).

## Note / possible follow-up

The manager accounts for allocations that pass through its `slmm_*` API. The
VM's own allocations are not yet routed through it, so a `.sleela` run currently
reports `allocs=0` in the exit stats. Routing the VM/GC allocations through the
manager would make the hard limit also cap a `.sleela` program's memory (not
just a native's). Not done here.
