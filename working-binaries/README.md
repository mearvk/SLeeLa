# working-binaries

Prebuilt native SLeeLa toolchain binaries, kept here for quick access.

| Binary | What it is | Size |
|--------|------------|------|
| `sleela` | `SLeeLa™ Language/Runtime 1.3.2-pro` — the authoritative SLeeLa compiler/runner. **Supported source syntax 1.0 – 3.1**, native C/C++ Core VM. Subcommands include `check`, `run`, `compile -o <out>`, `validate-artifact`, `nordshrift [--emit\|--roundtrip] [--target=…]`, `version`. **There is no separate `sleelac`** — compilation is `sleela compile`. | ~3.0 MB |
| `nordshrift` | `Nordshrift 2.7.1-dev` — the SLeeLa `.sst` transpiler driver (NS-SST-0001 spec 2.2); drives SLeeLa source syntax up to 3.1. Subcommands: `check`, `build`, `objects`, `relevance --target=java\|sleela\|c`, `version`. (The SLeeLa → Nordshrift → back round-trip is `sleela nordshrift`, inside the `sleela` binary.) | ~2.6 MB |

## Important caveats

- **Platform-specific.** These are **Linux x86-64**, dynamically linked ELF
  executables (`/lib64/ld-linux-x86-64.so.2`, glibc). They will **not** run on
  macOS, Windows, ARM, or musl-only systems. Rebuild from source for any other
  target.
- **Not the integrity-gated artifact.** The repository ships a SHA-256
  execution gate (`security/sha256-manifest.json`); the authoritative,
  reproducible way to obtain these tools is to **build from source**, not to
  trust a committed blob. These copies are a convenience for this environment,
  not a replacement for the verified build.
- Treat them as disposable. If they are stale or fail to run, rebuild.

## Build from source (authoritative)

```sh
# from the repository root
make compiler          # or: ./build-linux.sh
# output lands in impl/build/sleela and impl/build/nordshrift
```

## Quick use

```sh
./working-binaries/sleela version
./working-binaries/sleela check lib/idea/IdeaSystem.sleela
./working-binaries/sleela run   lib/idea/IdeaSystem.sleela

# or let the helper pick a binary / build on demand:
SLEELA_BIN=working-binaries/sleela tools/idea-check.sh
```

## Provenance

Built in the Kiro sandbox on 2026-11-12 from this repository's `impl/` tree
(`g++ -std=c++17 -O2`, pthread, `-fopenmp`; `make build/sleela` and
`make build/nordshrift`). VM ABI 1.8, 185 opcodes: this build includes the
Java UTF-16 String intrinsics (`javaStringFromUtf8`, `javaStringToUtf8`,
`javaStringFromCodeUnit`, ...), which the previous committed copy predated.
Rebuilt 2026-11-12 (`sleela` only) for the /lib loader rule that treats
`lib/java/jdk-28/` as a reference tier (`impl/frontend/driver.cpp`), so its
files never make a real `lib/java/java/...` class ambiguous.
Rebuilt 2026-11-12 (`sleela` and `nordshrift`) for the `Sleela.home` lineage
(SLEELA.syntax section 42: `parent`, `child`, `root`, `parents("---")` or `parents("...")`, `->`,
`super("constructor")`, `Sleela.VM|System|OS`, and the `Security2` boot).
Rebuilt 2026-11-13 (`sleela` and `nordshrift`) from `impl/` at the syntax 3.1 (3.01)
edition: character literals, array initializers, labeled `break`/`continue`,
`finally` on abrupt exit (SLEELA.syntax section 44), the `char` and array
specifications (sections 45, 46), and the Java-tier runtime changes through
SLEELA.syntax 43.7 and 37.5. Checked: `version` reports source syntax 1.0 – 3.1,
and a `#sleela 3.1` program using a char literal, an array initializer and
`a.length` checks and runs (the previous copy could not).
Rebuilt again 2026-11-13 for a VM performance fix, same versions: the plain String
intern table (`intern()` in `impl/core/sleela_core.c`) now has a hash index instead
of a linear scan over every String ever made. Behaviour and output are unchanged;
String-heavy programs no longer slow down as they run (the `lib/java/converter` run
on its 17-file JDK sample went from 66 s to 6.6 s). Regression check:
`impl/examples/string_intern_scaling.sleela`.

Rebuilt 2026-11-14, same versions: `read(handle)` no longer ends a piece inside a
UTF-8 character (`read_utf8_piece` in `impl/core/sleela_core.c`); an incomplete
tail waits for the next read. Before, a non-ASCII file could not be read safely
in pieces (`javaStringFromUtf8` stopped the VM on a split character). Regression
check: `impl/examples/read_utf8_pieces.sleela`.

Reported versions:

- `sleela` — `SLeeLa™ Language/Runtime 1.3.2-pro` (supported source syntax 1.0 – 3.1)
- `nordshrift` — `Nordshrift 2.7.1-dev` (NS-SST-0001 spec 2.2; drives SLeeLa up to 3.1)

SHA-256 of the committed copies:

```
8e2a38c422079b9d7e5693e109b2f53edc33a6bb8a5babb4c1d7a398c682a386  sleela
4acf57f2eb1815b906bdb32fb34bfba03e2163b073c7dd692f0dd07e9f48249a  nordshrift
```

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
