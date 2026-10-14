# working-binaries

Prebuilt native SLeeLa toolchain binaries, kept here for quick access.

| Binary | What it is | Size |
|--------|------------|------|
| `sleela` | The authoritative SLeeLa compiler/runner (Sleelvac™). Subcommands include `check`, `run`, `compile -o <out>`, `validate-artifact`, `nordshrift`, `version`. **There is no separate `sleelac`** — compilation is `sleela compile`. | ~1.7 MB |
| `nordshrift` | The Nordshrift round-trip tool (SLeeLa → Nordshrift → back; targets sleela/java/c). | ~1.4 MB |

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

Built in the Kiro sandbox from this repository's `impl/` tree with the
top-level `Makefile` (`g++ -std=c++17 -O2`, pthread). SHA-256 of the copies at
commit time is recorded in the pull request description.
