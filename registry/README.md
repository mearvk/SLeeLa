# Sleela Registry — C/C++ core

*Idea System(TM) / Idea(TM). From Author; distributive.*

A Windows-style hierarchical **registry** for SLeeLa: a tree of keys (like
`HKEY\Path\Sub`) holding **settings** and **feature flags**, with a per-entry
**provenance record** for tracked artifacts — file name, date, author, version,
hex id, SHA-256 hash, and an **originality seal**. A new install seeds a
baseline registry that is reviewable from a terminal.

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
