---
inclusion: always
---

# Sleelavirin™ — SLeeLa's own native antivirus engine

SLeeLa ships its **own** antivirus scanning engine in `antivirus/`. It is
original MEARVK code, portable **C11 / C++17**, with **no third-party AV engine
vendored** and **no runtime dependency** (ClamAV is only ever an optional
external *provider adapter*, never copied in — ClamAV's `libclamav` is GPLv2).

When antivirus, malware scanning, signatures, or file verification come up for
this repo, prefer and extend this native engine; do not propose adding or
embedding a third-party engine's source.

## What the native engine provides (`antivirus/src/`)

- **Whole-file SHA-256 hash matching** (exact) — `sleelavirin_native.c`.
- **Content byte-pattern matching** via a from-scratch **Aho–Corasick**
  automaton — `sleelavirin_ac.c` (+ `.slv` signature loader `sleelavirin_sig.c`).
- **Fuzzy similarity** (feature fingerprint + Hamming distance) and the
  **Trap-2 heuristic** — `sleelavirin_scan.c`, `sleelavirin_heuristic.c`.
- **Stable C bridge ABI** (request → validate → normalized verdict) —
  `sleelavirin_bridge.c`.
- **C++ wrapper library** (RAII, `std::optional`) — `sleelavirin_cpp.cpp`.
- **`sleelavirin-scan` CLI** — `--database` (hash) and `--content-db` (content).

Artifacts: `libsleelavirin.a` (C), `libsleelavirin_cpp.a` (C++), and
`sleelavirin-scan`.

## Honesty clause

A `CLEAN` / `NO_MATCH` result is **never** proof a file is safe. Keep the
fail-closed behavior (unreadable files/DBs and oversized inputs become `ERROR`,
never silent `CLEAN`).

## Build

Portable across **Windows XP+/7+/10+**, **macOS** (Intel & Apple Silicon), and
**Linux/Unix** from one source tree; OS differences live only in
`antivirus/src/sleelavirin_compat.h`.

```sh
cd antivirus
make            # POSIX GNU Make: C lib, C++ lib, tests, scanner
make test       # all C, C++, native, content, and Python tests
# or the universal path:
cmake -S . -B build && cmake --build build && ctest --test-dir build
```

- Windows: `./build-windows.ps1 [-WinXP] [-Test]`, or `nmake /f Makefile.win [WINXP=1]`.
- macOS: `./build-macos.sh [test]`.

## Integrity gate (important)

The `antivirus/src` + `antivirus/tests` C/C++ tree is **outside** the fail-closed
build manifest `security/sha256-manifest.json` (that gate covers `lib/**` and
`impl/`). But editing tracked files here DOES change the repo integrity records:
after changes, regenerate both and commit them alongside the source:

```sh
python3 tools/generate-important-sha256-manifest.py --root . --output security/important-sha256-manifest.json
bash tools/generate-all-sha256-md.sh   # updates SHA256-DIGESTS.md
```

Build outputs (`antivirus/build/`, `antivirus/cmbuild/`) are git-ignored.

## Reference docs (in `antivirus/`)

- `NATIVE-ENGINE.md` — build, usage, design of the C/C++ engine.
- `PLATFORMS.md` — full build/target matrix + Windows XP toolchain notes.
- `SCANNING-CONCEPTS-GLOSSARY.md` — the algorithms (Aho–Corasick, hashing, …).
- `CLAMAV-REFERENCE-ANALYSIS.md` — how a mature engine is structured (reference).
- `DEFINITIONS-SOURCING.md` — where definitions come from and how to build ours.
