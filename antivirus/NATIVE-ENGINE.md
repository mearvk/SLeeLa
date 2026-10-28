# Sleelavirin™ Native Engine (C / C++)

**Author:** Max Rupplin — MEARVK LLC — 2026
**Companion to:** `CLAMAV-REFERENCE-ANALYSIS.md`, `SCANNING-CONCEPTS-GLOSSARY.md`,
`DEFINITIONS-SOURCING.md`

A self-contained, portable, **license-clean** antivirus scanning engine for
SLeeLa users (and anyone who wants a bona fide, auditable AV core). It is
original MEARVK code: it copies **no** code from ClamAV or any other engine, has
**no** third-party runtime dependency, and builds with any C11 + C++17 compiler.

> **Honesty clause.** A `CLEAN` / `NO_MATCH` result is **never** proof that a file
> is safe. The native engine detects what its definitions describe; for broad
> coverage, pair it with an external provider (see `SCANNERS.md`).

---

## What it does

Two independent, composable detection methods over a file's bytes:

1. **Whole-file hash matching** — SHA-256 of the whole file vs. a hash index
   (exact match; zero false positives for that exact file).
2. **Content (byte-pattern) matching** — the native **Aho–Corasick** automaton
   finds any of many literal byte signatures in a single pass, so it catches
   known content even when the surrounding file differs (the capability pure
   hashing lacks).

Plus a **fuzzy** similarity check (feature fingerprint + Hamming distance) and a
**Trap-2 heuristic** scorer, both already present in the core.

---

## Components (`antivirus/src/`)

| File | Language | Role |
|---|---|---|
| `sleelavirin_native.{c,h}` | C11 | SHA-256 (file + buffer), hash-index file scan |
| `sleelavirin_scan.{c,h}` | C11 | known-file hash map, exact + fuzzy match, fingerprint |
| `sleelavirin_heuristic.{c,h}` | C11 | Trap-2 indicator scoring |
| `sleelavirin_ac.{c,h}` | C11 | **Aho–Corasick** multi-pattern content matcher |
| `sleelavirin_sig.{c,h}` | C11 | content-signature file format + loader |
| `sleelavirin_bridge.{c,h}` | C11 | stable C ABI: request → validate → verdict |
| `sleelavirin_cpp.{cpp,hpp}` | C++17 | ergonomic C++ wrapper (RAII, `std::optional`) |
| `sleelavirin_native_main.c` | C11 | the `sleelavirin-scan` CLI |

Artifacts produced by `make`:
- `build/libsleelavirin.a` — the C engine (static library).
- `build/libsleelavirin_cpp.a` — the C++ wrapper (static library).
- `build/sleelavirin-scan` — the command-line scanner.

---

## Build & test

```sh
cd antivirus
make           # build C lib, C++ lib, tests, and the scanner (POSIX GNU Make)
make test      # build + run all C, C++, native, content, and Python tests
make cpp-test  # just the C++ library test
CC=clang CXX=clang++ make   # use Clang
```

The engine builds warning-clean under `-O2 -Wall -Wextra -Wpedantic`
(C11 and C++17).

### Cross-platform

The engine compiles from one source tree on **Windows XP+/7+/10+**, **macOS**
(Intel & Apple Silicon), and **Linux/Unix**. OS differences live only in
`src/sleelavirin_compat.h`. Build options:

- **CMake (universal):** `cmake -S . -B build && cmake --build build` — add
  `-DSLEELAVIRIN_WINXP=ON` for the Windows XP floor, or
  `-DCMAKE_OSX_DEPLOYMENT_TARGET=10.9` for older macOS.
- **Windows:** `./build-windows.ps1 [-WinXP] [-Test]` (CMake or MinGW-w64), or
  `nmake /f Makefile.win [WINXP=1] [test]` (MSVC).
- **macOS:** `./build-macos.sh [test]` (Apple Clang).

See **`PLATFORMS.md`** for the full matrix and the Windows XP toolchain notes.

---

## Using the CLI

```sh
# Whole-file hash scan:
build/sleelavirin-scan --database signatures/native-sha256.txt file1 file2

# Content (byte-pattern) scan:
build/sleelavirin-scan --content-db tests/content-signatures.slv suspect.bin

# Both at once (each pass reported independently):
build/sleelavirin-scan --database H.txt --content-db C.slv target
```

Exit codes: `0` = no match for any target, `1` = at least one detection,
`2` = a usage error or an unreadable file/database. Output lines are
`DETECTED <path> : <detail>` or `NO_MATCH <path>`.

---

## Content-signature format (`.slv`)

A Sleelavirin-specified text format — a deliberately small literal-byte subset
(Tier 3 of `DEFINITIONS-SOURCING.md`), **not** copied from ClamAV's `.ndb`:

```
# comment; blank lines ignored
<name>:<severity>:<flags>:<hex-bytes>
```

- `name` — 1..127 chars, no `:` (the threat/family name)
- `severity` — decimal `0..255`
- `flags` — comma-separated; currently only `nocase` (ASCII-case-insensitive)
- `hex-bytes` — even number of hex digits (spaces allowed), e.g. `4d 5a 90 00`

Example (`tests/content-signatures.slv`): matches a fragment of the **EICAR**
standard AV test string — a safe, industry-standard detection-pipeline marker,
*not* real malware.

---

## Using the C library

```c
#include "sleelavirin_bridge.h"

sleelavirin_bridge_request_t req; sleelavirin_bridge_result_t res;
memset(&req, 0, sizeof req);
snprintf(req.target_path,   sizeof req.target_path,   "%s", path);
snprintf(req.database_path, sizeof req.database_path, "%s", hash_index);
snprintf(req.engine_name,   sizeof req.engine_name,   "native");
req.scope = SLEELAVIRIN_SCOPE_LOCAL;
req.max_file_size_bytes = 256ull * 1024 * 1024;

if (sleelavirin_bridge_validate_request(&req) == 0 &&
    sleelavirin_bridge_scan_native(&req, &res) == 0) {
    printf("%s (%u detections): %s\n", res.verdict, res.detections, res.detail);
}
```

Direct Aho–Corasick use:

```c
#include "sleelavirin_sig.h"
sleelavirin_sig_stats_t st;
sleelavirin_ac_t *ac = sleelavirin_sig_load_file("defs.slv", &st);   /* compiled */
long n = sleelavirin_ac_scan(ac, buffer, length, on_match, userdata);
sleelavirin_ac_destroy(ac);
```

Link against `build/libsleelavirin.a`.

---

## Using the C++ library

```cpp
#include "sleelavirin_cpp.hpp"

sleelavirin::HashMap map(1024);
sleelavirin::KnownFile k;
k.name = "Demo"; k.provider = "native"; k.rule_id = "R-1"; k.severity = 8;
k.fingerprint = *sleelavirin::FileFingerprint::from_file("known_bad.bin");
map.add(k);

if (auto fp = sleelavirin::FileFingerprint::from_file("suspect.bin"))
    if (auto m = map.scan(*fp))
        std::cout << static_cast<int>(m->confidence) << " " << m->known_name << "\n";
```

Link against `build/libsleelavirin_cpp.a` **and** `build/libsleelavirin.a`.

---

## Design notes & algorithms

- **Aho–Corasick** (`sleelavirin_ac.c`): trie + BFS failure links + output links;
  one pass, O(n + matches), independent of pattern count. Case-insensitive
  patterns are kept in a **separate** single-case automaton scanned over a
  lowercased copy of the buffer, so case-sensitive patterns are never corrupted.
  (See `SCANNING-CONCEPTS-GLOSSARY.md` for the theory.)
- **SHA-256** (`sleelavirin_native.c`): a clean-room FIPS 180-4 implementation,
  no OpenSSL dependency.
- **Compile-once / scan-many:** signatures are parsed and compiled into the
  automaton once (`sleelavirin_sig_load_*`), then only walked per file.
- **Fail-closed:** unreadable files/DBs and oversized targets become `ERROR`
  verdicts, never silent `CLEAN`.

---

## Scope & limits (honest boundaries)

- Content matching supports **literal** byte patterns (plus `nocase`). Wildcards,
  anchored byte ranges, regex, archive unpacking, and PE/section parsing are
  **out of scope** for this native engine — by design. For full coverage use
  ClamAV as an external provider (`SCANNERS.md`).
- Definitions should be sourced per `DEFINITIONS-SOURCING.md` (first-party hashes
  and content patterns you are entitled to ship; permitted external feeds with
  recorded provenance).
