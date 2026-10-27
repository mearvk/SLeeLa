# ClamAV Engine — Coding & Structural Strategy (Reference Analysis)

**Author:** Max Rupplin — MEARVK LLC — 2026
**Scope:** This is a *reference-only* study of the ClamAV engine as found in the
`mearvk/ClamAV.US.Legal.Edition` repository. It documents the coding conventions
and structural strategy that ClamAV uses to scan files and decide a byte buffer
matches a virus. Nothing here is a port, a copy, or a dependency. It exists so
the Sleelavirin™ native matcher can learn from a mature design while remaining
small, self-contained, and license-clean.

> **License note.** ClamAV `libclamav` is GPLv2. Sleelavirin does **not** vendor
> any ClamAV source. The upstream engine is only ever reached as an *external
> provider/adapter* (see `SCANNERS.md`). This document describes architecture and
> technique; it reproduces no substantive source.

---

## 1. What "the engine" actually is

ClamAV's detection logic lives almost entirely in one library, **`libclamav/`**.
The rest of the tree (`clamscan/`, `clamd/`, `freshclam/`, `sigtool/`, the Rust
crate, CMake, vcpkg) is CLI front-ends, a daemon, database updating, tooling, and
build plumbing around that one library.

"Full coverage" of the scanning mechanism is therefore effectively *the whole
`libclamav` library*, not a liftable snippet. The matcher subsystem alone is on
the order of **~15,000 lines of C**, and it depends on database loading, the
`fmap` memory abstraction, the engine object, bytecode, and file-type parsers.

### Core matcher files (the "match → it's a virus" path)

| File | ~Lines | Responsibility |
|---|---:|---|
| `matcher.c` | 1,527 | Scan **dispatch core** — picks roots, runs the matcher cascade |
| `matcher.h` | 395 | Central structs: `cli_matcher`, `cli_lsig_tdb`, `cli_ac_lsig` |
| `matcher-ac.c` | 3,249 | **Aho–Corasick** multi-pattern automaton (the primary engine) |
| `matcher-bm.c` | 426 | **Boyer–Moore** single-pattern fast path |
| `matcher-byte-comp.c` | 1,013 | Byte-comparison subsignatures |
| `matcher-pcre.c` | 781 | PCRE regular-expression matching |
| `matcher-hash.c` | 467 | MD5/SHA hash signatures (whole-file / section) |
| `filetypes.c` | ~700 | File-type identification feeding target-specific roots |
| `hashtab.c` | ~1,000 | Hash-table primitives used throughout |
| `scanners.c` | 6,611 | **Recursive** container/archive unpacking + scan orchestration |
| `readdb.c` | large | Signature **database loading** into in-memory roots |

---

## 2. The procedural scanning mechanism

The "scan bytes, know you matched a virus" mechanism is a **cascade of
specialized matchers run over a byte buffer**, coordinated by one static
dispatcher.

```
cl_scanfile / cl_scandesc            (public API)
        |
cli_magic_scan_desc  -> cli_magic_scan   (type ID + recursion)
        |
cli_scan_buff        (matcher.c: choose signature roots)
        |
matcher_run          (matcher.c: THE procedural core)
        |
  +-----+-------------------+----------------+------------------+
  |     |                   |                |                  |
pre-   Boyer-Moore      Aho-Corasick     byte-compare /      PCRE
filter cli_bm_scanbuff  cli_ac_scanbuff  fuzzy image hash    cli_pcre_scanbuf
  |
  on CL_VIRUS -> cli_append_virus(ctx, virname)   <-- "this is a virus"
```

### Step by step (`matcher.c`)

1. **Prefilter.** If the root has a `filter`, `filter_search_ext()` finds the
   earliest plausible match position so most of the buffer can be skipped. A
   safety tail of `maxpatlen` bytes is always scanned so a signature straddling
   the cut is never missed.
2. **Boyer–Moore** (`cli_bm_scanbuff`) runs first unless the root is `ac_only` —
   a fast skip-table scan for literal patterns.
3. **Aho–Corasick** (`cli_ac_scanbuff`) is the main engine: one pass over the
   buffer walks a trie that matches *all* literal sub-patterns simultaneously.
4. **Byte-compare** metasignatures (`cli_bcomp_scanbuf`) run if present.
5. **Fuzzy image hash** is checked for graphics types (GIF/TIFF/JPEG/PNG).
6. **PCRE** (`cli_pcre_scanbuf`) runs last, because regex subsignatures feed
   *logical* signatures that can only be evaluated after all literal subsigs are
   known; it may need the whole buffer mapped in.
7. **Verdict.** Any matcher returning `CL_VIRUS` causes
   `cli_append_virus(ctx, virname)` to record the hit. The function distinguishes
   a *virus* result from a *file-type* result (`AC_SCAN_VIR` vs `AC_SCAN_FT`) —
   the same automaton is reused both to classify file types and to detect malware.

### Target-specific vs generic roots

`cli_scan_buff()` keeps signatures in multiple **roots** (`engine->root[i]`). It
first matches against the root specialized for the detected file type
(`cli_mtargets` maps type → root index), then against the **generic** root
`engine->root[0]`. This limits each scan to only the signatures relevant to that
file type — a scalability strategy, not just an optimization.

---

## 3. Structural strategy (the patterns worth learning)

### 3.1 Compile signatures once, scan many times
Signatures are text (`.ndb`, `.ldb`, `.hdb`, `.cdb`, YARA, …). At load time
`readdb.c`'s `cli_load*` family parses each format and **compiles** it into
in-memory automatons (AC trie, BM shift tables, hash tables). `cli_ac_buildtrie()`
finalizes the Aho–Corasick failure links. The hot scan path then only *walks*
prebuilt structures — no parsing per file. This separation (load/compile vs. run)
is the single most important design choice.

### 3.2 One dispatcher, many pluggable matchers
`matcher_run()` is a thin, fixed cascade; each matcher (`bm`, `ac`, `bcomp`,
`pcre`, `hash`, `fuzzy`) is an independent module with its own `*.c/.h` pair and a
uniform `*_scanbuff(buffer, length, …) -> cl_error_t` shape. New matcher families
are added as new modules, not by editing the others.

### 3.3 The `cli_matcher` "root" aggregates every matcher's state
`struct cli_matcher` (in `matcher.h`) is the compiled database for one target. It
holds, side by side: BM shift/suffix tables; the AC trie (`ac_root`, node/list/
pattern tables, `mindepth`/`maxdepth`); hash tables; the prefilter `filter`;
PCRE metatable; byte-compare metatable; the fuzzy hashmap; and `maxpatlen` /
`ac_only` tuning flags. One struct = one fully-compiled matcher for one file type.

### 3.4 Logical signatures compose simple subsignatures
A logical signature (`cli_ac_lsig` + `cli_lsig_tdb`) fires only when a boolean
expression over its subsignatures is satisfied (e.g. "subsig 0 AND (1 OR 2)",
with target type, file-size, and offset constraints). Per-scan match state lives
in `cli_ac_data` (`lsigcnt`, offset matrices, macro match tracking). This lets
simple literal patterns be *composed* into precise, low-false-positive detections
without writing bespoke code per malware family.

### 3.5 Scan state is threaded explicitly through `cli_ctx`
There are no globals on the scan path. A single `cli_ctx` (in `others.h`) carries
everything: the `engine`, current `fmap`, scan `options`, counters (`scansize`,
`scannedfiles`), evidence, JSON metadata, a `time_limit`, and an `abort_scan`
flag. Passing one context pointer keeps the engine **thread-safe** (multiple
scans share a read-only engine) and makes limits/cancellation uniform.

### 3.6 Recursion is an explicit bounded stack, not C recursion
Archives contain archives (zip → tar → exe → …). ClamAV models this with an
explicit **recursion stack** on `cli_ctx`: `recursion_stack[]`,
`recursion_stack_size` (== `engine->max_recursion_level`), and `recursion_level`.
`scanners.c` pushes a new layer (a new `fmap` over the extracted child) before
re-entering the matcher and pops on return. Depth is *bounded by data*, so a
malicious deeply-nested archive cannot blow the C stack — a key hardening idea.

### 3.7 `fmap` abstracts "the bytes" regardless of source
Everything scanned is an `fmap_t` (a file, a memory buffer, or a window into a
parent fmap). Matchers never touch file descriptors directly; they ask the fmap
for bytes (`fmap_need_off_once`, etc.). The same matcher code therefore scans a
file on disk, an in-memory buffer, or a nested region inside an archive.

### 3.8 Memory pooling
When built with `USE_MPOOL`, compiled signature structures are allocated from a
per-engine `mpool_t` (note the `mempool` members in `cli_matcher` /
`cli_lsig_tdb`). The huge, long-lived signature set is allocated and freed as a
unit, reducing fragmentation and teardown cost.

### 3.9 Uniform `cl_error_t` return discipline
Every function returns `cl_error_t`. The scan path overloads it meaningfully:
`CL_CLEAN` / `CL_SUCCESS`, `CL_VIRUS` (reported, scanning may continue to collect
all names), `CL_BREAK`, resource errors (`CL_EMEM`, `CL_EMAXSIZE`, `CL_ETIMEOUT`).
A `saved_ret` idiom preserves a pending type/virus result while a later stage
runs. Control flow is driven entirely by these codes.

### 3.10 Coding conventions observed
- **Module = `name.c` + `name.h`**; internal API prefixed `cli_`, public API
  prefixed `cl_` (two namespaces: private engine vs. published `libclamav` API).
- Fixed-width integer types (`uint32_t`, `off_t`), `clang-format` enforced
  (`.clang-format` + `clam-format` wrapper in the repo).
- GPLv2 header block on every file; authorship/provenance retained.
- Optional subsystems fenced with `#ifdef` (`HAVE_YARA`, `USE_MPOOL`,
  `CLI_PERF_LOGGING`, `CL_THREAD_SAFE`) so the build scales down cleanly.
- Defensive `NULL` checks returning `CL_ENULLARG`; `cli_dbgmsg`/`cli_errmsg`
  logging rather than `printf`.

---

## 4. The "US.Legal.Edition" fork additions

On top of stock ClamAV this fork adds a small, **advisory-only** procedural
scaffold (`gating/`, `herald/`, `agent/`, `PROCEDURAL_GATING.md`,
`PROCEDURAL_CAUSATION.hss`). It layers eight "sequitur" levels
(provenance → confirmation → tracing → qualification → causal attachment →
attenuation → qualified closure → operator herald). Its own rules state it
**must never** downgrade a ClamAV detection to "safe," and that *the ClamAV
engine remains the detection authority*. The gating code is tiny (e.g.
`gating/1/gate.c` ≈ 97 lines) and deliberately decoupled from `libclamav`.

**Takeaway for Sleelavirin:** the fork itself follows the same philosophy we
already adopted — keep the heavyweight detection engine as an authority behind a
thin, reviewable, additive layer, rather than rewriting it.

---

## 5. How this maps onto Sleelavirin™ (`antivirus/`)

Our native core (`src/sleelavirin_scan.c`, ~127 lines) already mirrors ClamAV's
*spirit* at a fraction of the size:

| Concern | ClamAV | Sleelavirin native |
|---|---|---|
| Compiled DB in memory | `cli_matcher` roots (trie + tables) | `sleelavirin_hashmap_t` (open-addressed) |
| Exact match | AC/BM/hash → `CL_VIRUS` | SHA-256 whole-file → `SLEELAVIRIN_EXACT_MATCH` (conf 100) |
| Approximate match | fuzzy image hash | 64-bit feature mask + Hamming distance → `SLEELAVIRIN_SEMI_MATCH` |
| Verdict discipline | `cl_error_t` codes | `sleelavirin_match_t.kind` + confidence |
| "absence ≠ clean" | explicit in docs/flow | explicit: `SLEELAVIRIN_UNKNOWN`, comment in `scan.c` |
| Full signature coverage | native | **external provider adapter** (`SCANNERS.md`, `tools/*clamav*.py`) |

**The one real capability gap** vs. ClamAV is literal *byte-pattern* matching
inside file contents (AC/BM), as opposed to whole-file hashing. If we ever want
to close it natively (license-clean), the lesson from this study is to add it as
an independent matcher module in the §3.2 shape —
`sleelavirin_ac.c` / `sleelavirin_bm.c` with a `*_scanbuff(buf, len) -> verdict`
signature — and keep ClamAV as the provider for full `.cvd`/`.cld` coverage.

---

## 6. One-paragraph summary

ClamAV scans by **compiling** a large text signature set into per-file-type
in-memory automatons at load time, then running each byte buffer through a fixed
**cascade of pluggable matchers** (prefilter → Boyer–Moore → Aho–Corasick →
byte-compare → fuzzy → PCRE) driven by a single dispatcher, `matcher_run()`. A
match calls `cli_append_virus()` to name the threat. All state is threaded
through one explicit `cli_ctx`; archive nesting is handled by a **bounded
recursion stack** over `fmap` byte-sources; and everything communicates via
`cl_error_t`. "Full coverage" is the entire GPLv2 `libclamav` library, so the
correct strategy for Sleelavirin is the one already in place: a small
self-contained native matcher plus ClamAV as an external provider — never a
vendored copy.
