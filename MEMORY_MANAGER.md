# SLeeLa Memory Manager & Native Executable Launcher

This document describes two related additions to the SLeeLa runtime:

1. the **Memory Manager** — a flag-enabled facility that accounts for raw
   process memory and can fail allocations closed at a hard byte limit; and
2. the **native executable launcher** — running an arbitrary native OS
   executable from the SLeeLa terminal, with the Memory Manager engaged
   on-need.

They are complementary: the launcher is the workload most likely to exhaust
host memory, so it turns the Memory Manager on automatically.

---

## 1. The Memory Manager

### Where it sits

| Layer | File | Role |
|---|---|---|
| OS allocator abstraction | [`impl/core/sleela_memory.c`](impl/core/sleela_memory.c) | platform `malloc`/`calloc`/`realloc`/`free`, aligned alloc, page size, monotonic time — *no* accounting (see [`impl/MEMORY_SYSTEM_OS.md`](impl/MEMORY_SYSTEM_OS.md)) |
| **Memory Manager** | [`impl/core/sleela_memmgr.c`](impl/core/sleela_memmgr.c) | **accounting + limits** on top of the allocator |
| Mark-sweep GC | [`runtime/garbage_collector.c`](runtime/garbage_collector.c) | tracks *managed objects* (byte threshold) |
| Quota supervisor | [`runtime/security_supervisor.c`](runtime/security_supervisor.c) | per-*class*/*role* instance & byte budgets |

The Memory Manager is deliberately independent of the GC and the quota
supervisor. Those account for *managed objects* and *class/role* budgets; the
Memory Manager accounts for *raw process memory* — the axis that matters when
SLeeLa runs an arbitrary native executable.

> **How the VM's own object lifecycle works (important).** All three facilities
> above — the Memory Manager, the mark-sweep GC, and the quota supervisor — are
> **opt-in peripheral services**. The SLeeLa interpreter core does **not**
> GC-manage its own runtime values. `SLValue`s are plain tagged unions; strings
> are interned and freed once at `slvm_free`; struct instances live in a fixed
> `SL_MAX_STRUCTS` slot array reclaimed only at VM teardown. So a running SLeeLa
> program is **not** garbage-collected by default — the GC in
> [`runtime/garbage_collector.c`](runtime/garbage_collector.c) is a standalone
> service (exercised by `make test-runtime`) that a host embeds deliberately, not
> the engine's allocator. Do not assume the core is GC-backed.
>
> Only the C sources (`.c`/`.h`) in `runtime/` are part of the build; the former
> `.cpp`/`.hpp` duplicates of the GC and quota supervisor have been removed to
> avoid the impression of two live implementations.

### What it does

The manager is **enabled by default** for SLeeLa execution with a hard **2 GiB** ceiling. The configured ceiling may be reduced to **256 MiB**. When disabled (the default) the wrappers forward
straight through to the OS allocator with no accounting overhead, so linking it
in costs nothing until it is enabled.

When enabled it:

- prefixes every block with a small aligned header recording the payload size,
  so `free`/`realloc` update the running total **exactly** with no side table;
- tracks **live bytes**, **peak bytes**, and **live / total allocation counts**;
- enforces an optional **hard byte limit**, failing an allocation *closed*
  (returns `NULL`, records a refusal) rather than over-committing the host;
- is **thread-safe** (a single manager mutex via the `slthread` abstraction),
  so it can back the VM and a spawned native at the same time.

### API (`impl/core/sleela_memmgr.h`)

```c
int    slmm_enable(size_t limit_bytes);   /* 0 selects the default 2 GiB ceiling        */
void   slmm_disable(void);
int    slmm_is_enabled(void);
size_t slmm_limit(void);
void   slmm_set_limit(size_t limit_bytes);

void*  slmm_alloc(size_t size);
void*  slmm_calloc(size_t count, size_t size);
void*  slmm_realloc(void* ptr, size_t size);
void   slmm_free(void* ptr);

void        slmm_stats(SLMMStats* out);
SLMMStatus  slmm_last_status(void);
const char* slmm_status_name(SLMMStatus s);
void        slmm_reset(void);
int         slmm_format_report(char* buf, size_t cap);
```

`SLMMStatus` distinguishes `OK`, `DISABLED`, `LIMIT` (would exceed the hard
limit), `OOM` (the OS allocator returned `NULL`), and `OVERFLOW` (a size
computation overflowed).

> **Ownership rule.** Every pointer returned by `slmm_*` carries a header and
> **must** be released with `slmm_free` / resized with `slmm_realloc` — never
> the raw C `free()`.

### Enabling it from the command line

A leading `--memory-manager` (or `--mm`) option turns it on, before the
subcommand:

```sh
sleela --memory-manager run examples/hello.sleela
sleela --memory-manager=512M run program.sleela     # hard limit, K/M/G suffixes
SLEELA_MEMORY_MANAGER=512M sleela run program.sleela # or via the environment
```

`<size>` accepts a plain byte count or a `K`/`M`/`G` suffix (optionally with a
trailing `B`, e.g. `128KB`). `--memory-manager` with no value is unlimited
(accounting only). `SLEELA_MEMORY_MANAGER=off|0` disables it.

When enabled, a one-block report is printed to stderr at exit:

```
[memory-manager] backend=linux-posix enabled=yes limit=67108864 bytes
[memory-manager] live=0 bytes  peak=131072 bytes  live-allocs=0
[memory-manager] allocs=42  frees=42  refused=0  last=ok
```

---

## 2. Running a native executable from the terminal

The launcher runs a **native OS executable** from the SLeeLa terminal under a
real pseudo-terminal — the same OS-aware PTY primitive
([`impl/core/sleela_terminal.c`](impl/core/sleela_terminal.c)) the runtime uses,
with POSIX (`fork`+`execl` via `/bin/sh -c`) and Windows (ConPTY +
`CreateProcessW`) backends.

```sh
sleela native [--memory-manager[=<size>]] [--] <program> [args...]
sleela exec   [--memory-manager[=<size>]] [--] <program> [args...]

# via the OS-terminal launcher:
SLeeLa native /usr/bin/env
SLeeLa exec --memory-manager=128M ./my-tool --flag value
```

- The operand is a **program name/path**, not a `.sleela` object, so the
  `SLeeLa` launcher passes it through verbatim (no object resolution).
- Every argument is shell-quoted before being handed to the PTY.
- The child's output is relayed to stdout until it exits.
- The run honors the same **SHA-256 execution gate** as `run`/`check`/`compile`.
- The **Memory Manager is enabled automatically** ("on need") for the duration;
  `--memory-manager=<size>` sets a hard limit, otherwise it runs unlimited.

Use `--` to stop option parsing when the native program takes options that
would otherwise look like launcher options.

---

## Build & test

Both binaries link the manager (`impl/Makefile`):

```sh
cd impl && make            # builds sleela + nordshrift with the Memory Manager
make test-memmgr           # behavioral smoke test for the manager
make test                  # full suite (includes test-memmgr)
```

The behavioral test [`impl/tests/core/memmgr_smoke.c`](impl/tests/core/memmgr_smoke.c)
asserts real behavior: untracked-when-disabled, exact byte/alloc accounting
across `alloc`/`calloc`(zeroed)/`realloc`(preserving)/`free`, and a hard limit
that fails allocation closed.
