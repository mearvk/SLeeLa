# lib/registry — Sleela OOD equivalents of the registry

*Idea System(TM) / Idea(TM). From Author; distributive.*

The Sleela object-oriented mirror of the native registry core
([`/registry`](../../registry/README.md)), so a program can model, seed, and
review the registry from Sleela.

The native core these classes mirror is built and its baseline table
materialized by the **master build** (repo-root `make all` → `registry` target
→ `build-registry.sh`); see [`/registry`](../../registry/README.md) for the
build and the on-disk `.sleelareg` / MySQL table it produces.

| Class | Mirrors (native) | Role |
|-------|------------------|------|
| `SLRegistryValue` | a registry value (`sl_reg_kind`) | one setting / feature flag: string, int, or bool |
| `SLRegistryProvenance` | `sl_reg_provenance` + seal | a tracked artifact's file/date/author/version/hex-id/hash + **originality seal** |
| `SLRegistryKey` | the key node | a path-keyed node holding values + an optional provenance record |
| `SLRegistry` | the store | the HKEY-style tree; `seedDefaults()` + `show()` (terminal review) |

## Shape

Following the lib conventions: `#sleela 1.3`, the five-field doc-header,
`configure()`-style init, and **numbered fields instead of arrays** (the surface
has no arrays) for the bounded value/key sets.

```
SLRegistry reg = new SLRegistry();
reg.seedDefaults();     // System, System/VM, Settings, Features, Artifacts
reg.show();             // terminal review
boolean on = reg.featureEnabled("modium");
```

## The originality seal

`SLRegistryProvenance` records the seal input `hexId | version | hash | author`.
The SHA-256 of that (and of the file content for `hash`) is computed by the host
across the native bridge; `isSealed()` / `verify(expected)` check it. The native
`/registry` core is the authority for computing and persisting seals.

## Naming notes

A few method names were chosen to avoid words the grammar treats as contextual
in call position: the review method is `show()` (not `review`), and the
seal-check is `isSealed()` (not `sealed`). Chained calls on a method's return
value are also avoided (assign to a local first) — the SLeeLa surface rejects
`a.foo().bar()`.
