# working-binaries

Prebuilt native SLeeLa toolchain binaries, kept here for quick access.

| Binary | What it is | Size |
|--------|------------|------|
| `sleela` | `SLeeLa™ Language/Runtime 0.3.30-dev` — the authoritative SLeeLa compiler/runner. **Supported source syntax 1.0 – 3.0**, native C/C++ Core VM. Subcommands include `check`, `run`, `compile -o <out>`, `validate-artifact`, `nordshrift [--emit\|--roundtrip] [--target=…]`, `version`. **There is no separate `sleelac`** — compilation is `sleela compile`. | ~2.6 MB |
| `nordshrift` | `Nordshrift 2.7.1-dev` — the SLeeLa `.sst` transpiler driver (NS-SST-0001 spec 2.2); drives SLeeLa source syntax up to 3.0. Subcommands: `check`, `build`, `objects`, `relevance --target=java\|sleela\|c`, `version`. (The SLeeLa → Nordshrift → back round-trip is `sleela nordshrift`, inside the `sleela` binary.) | ~2.3 MB |

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

Built in the Kiro sandbox from this repository's `impl/` tree
(`g++ -std=c++17 -O2`, pthread; `make build/sleela` and `make build/nordshrift`).
Reported versions:

- `sleela` — `SLeeLa™ Language/Runtime 0.3.30-dev` (supported source syntax 1.0 – 3.0)
- `nordshrift` — `Nordshrift 2.7.1-dev` (NS-SST-0001 spec 2.2; drives SLeeLa up to 3.0)

SHA-256 of the committed copies:

```
d29e85a78b13b9a6e58659e73b4d2457021e1ce33961808374267b873a96e622  sleela
a9e70651a70a86959736c62e59806870af621396c1609a5b544f91f0a447e1b3  nordshrift
```

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
