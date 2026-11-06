# Sleela Registry — C/C++ core

*Idea System(TM) / Idea(TM). From Author; distributive.*

A Windows-style hierarchical **registry** for SLeeLa: a tree of keys (like
`HKEY\Path\Sub`) holding **settings** and **feature flags**, with a per-entry
**provenance record** for tracked artifacts — file name, date, author, version,
hex id, SHA-256 hash, and an **originality seal**. A new install seeds a
baseline registry that is reviewable from a terminal.

## Architecture and compatibility

- [Registry 3.0 architecture](REGISTRY-3.0-ARCHITECTURE.md) — canonical design and staged delivery contract.
- Current native C ABI and C++ facade remain the compatibility implementation; SLREG 1 remains supported until the separately tested SLREG 2 migration ships.
- Modium remains responsible for storage mechanics and integrity checks. Registry catalogs region identities and relationships; it never claims or writes a device as a side effect of lookup or review.

## Registry 3.0 record engine

The first native record implementation is in [v3/](v3/README.md). It is introduced alongside the original C ABI rather than replacing the existing store in place:
- stable 128-bit record IDs derived from SHA-256 seeds or supplied explicitly;
- namespaces, record kinds, aliases, typed relationships, lifecycle state, and optimistic revision checks;
- deterministic, percent-escaped SLREG 2 records with UTF-8 validation and atomic replacement;
- explicit SLREG 1 migration that preserves keys, typed values, hierarchy references, and the original seven provenance fields;
- a C++17 RAII facade, the separate sleela-registry3 CLI, and a Modium region catalog operation.

Run focused checks with make -C registry test or make registry-test. The CMake/CTest matrix is defined in registry/CMakeLists.txt and .github/workflows/registry-v3.yml.

The original SLREG 1 store and terminal remain the compatibility interface. The new code does not silently migrate or rewrite an existing store. For the integration boundary see [Registry 3.0 bindings](REGISTRY-3.0-BINDINGS.md) and [Modium catalog contract](REGISTRY-3.0-MODIUM.md).

## Manual democratic voting from SLeeLa source documents

The proposed inert source annotation and a careful manual secret-ballot process are documented in [DEMOCRATIC-VOTING-SYNTAX.md](DEMOCRATIC-VOTING-SYNTAX.md). The current compiler ignores the comment annotation; this is a declarative governance design, not an implemented public-election system.

## Files

| File | Role |
|------|------|
| `sleela_registry.h`   | public C ABI (keys, values, provenance, seal, persistence, review) |
| `sleela_registry.c`   | implementation — sorted key store, self-contained SHA-256, originality seal, `.sleelareg` text format |
| `sleela_registry.hpp` | RAII C++ facade (`reg::Registry`) |
| `sleela_registry_tool.cpp` | terminal reviewer (`seed` / `review`) |
| `Makefile` | build the core + reviewer |

## Build & review

The registry builds **as part of the master build**: the repo-root `make all`
runs the `registry` target, which fires [`../build-registry.sh`](../build-registry.sh)
to build this core and **materialize the baseline registry table** a fresh
install reviews — the portable text store `build/sleela.sleelareg` plus the
MySQL table script `build/registry.sql` (exported via `sleela registry export`).

```sh
# from the repo root — part of the full build:
make all                          # ... core, vm, ..., registry
make registry                     # just the registry + table materialize
./build-registry.sh               # the same, invoked directly

# standalone, from this folder:
cd registry && make run           # seed a demo registry and review it
./build/sleela-registry review build/demo.sleelareg
```

Example review:

```
Sleela Registry 1.0.0  (keys=6 values=9 provenance=1)
[Artifacts/registry-core]
    <provenance> file=sleela_registry.c author=... version=1.0.0 hexid=1dea0005ec0de001
                 hash=0db65e63... seal=3816ea8f... [SEALED]
[Features]
    modium = true
    vmSelect = true
    premiumVms = true
[Settings]
    memoryLimitMiB = 2048
    threadCap = 128
    verbosity = "normal"
[System]
    name = "Idea System(TM) / SLeeLa"
...
```

## The originality seal

For a tracked artifact, the seal is `SHA-256(hexId | version | hash | author)`.
If any of those identity-or-content fields is altered, `sl_reg_seal_verify`
reports the seal **BROKEN**. The content `hash` is SHA-256 of the file itself,
so tampering with either the file or its recorded identity is visible.

## On-disk format (`.sleelareg`)

Portable, deterministic, diffable text:

```
SLREG 1
[Settings]
= i memoryLimitMiB 2048
= s verbosity normal
[Artifacts/registry-core]
@ file | date | author | version | hexid | hash | seal
```

## Sleela equivalents

The OOD mirror lives in [`../lib/registry`](../lib/registry/) — `SLRegistry`,
`SLRegistryKey`, `SLRegistryValue`, `SLRegistryProvenance` — for programs that
read or review the registry from Sleela.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
