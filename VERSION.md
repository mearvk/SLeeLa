# Sleela — Version

This file is the single place of record for the versions of everything in this
repository. Sleela versions several things independently (the metadocument
distinguishes language syntax, language semantics, the implementation, and the
tooling), so they do not share one number. Update this file whenever any of the
versions below changes, and keep it consistent with the source-of-truth
locations listed in each row.

## Current versions (at a glance)

| Component | Version | Status | Source of truth |
|-----------|---------|--------|-----------------|
| **Sleela toolchain / implementation** (`sleela` CLI) | **0.1.2** | Working (pre-1.0) | `impl/frontend/driver.cpp` (`kVersion`) |
| **Sleela language syntax** | **1.0** | Supported range `1.0 .. 1.0` | `impl/frontend/version.h` (`min`/`maxSupportedSyntax`) |
| **Nordshrift** (`.sst` transpiler driver) | **1.0** | Working | `impl/nordshrift/nordshrift.cpp` (`kVersion`) |
| **NS-SST-0001** (`.sst` format specification) | **1.0.0** | Normative | `SST.model` (Revision) |
| **SL-META-0001** (Sleela Language Metadocument) | **1.0.0** | Pre-Normative | `src/Sleela.manifest` (Revision) |

> **Short answer:** the Sleela we have here is the **0.1.2** toolchain,
> implementing **Sleela syntax 1.0**, driven by **Nordshrift 1.0**.

## What each version means

### Sleela toolchain / implementation — `0.1.2`
The version of the actual C/C++ implementation in `impl/` (the `sleela` CLI:
lexer → parser → compiler → C core). This is a semantic `MAJOR.MINOR.PATCH`
number and is what `./build/sleela version` reports. It is **pre-1.0**: the
end-to-end pipeline works (recursion, loops, arithmetic, strings, I/O,
threading, conducted methods, `.xclass` ingest, and version-aware compilation),
but the language is not yet frozen.

### Sleela language syntax — `1.0`
The grammar version a `.sleela` file (a **Wrapper™**) declares with its
`#sleela MAJOR.MINOR` pragma (SL-META-0001 §4.4). The compiler is **version
aware**: it accepts any declared version within its supported range
(`1.0 .. 1.0` today) and rejects anything newer or malformed. Syntax is
versioned independently of the implementation — the `0.1.2` toolchain
implements syntax `1.0`.

Query the live supported range:

```sh
./build/sleela version
#  Sleela 0.1.2 (C/C++ core; SHEET.sheet conducted methods; .xclass input)
#    supported .sleela syntax: 1.0 .. 1.0 (declare per-file with '#sleela 1.0')
```

### Nordshrift — `1.0`
The `.sst`-driven transpiler that emits the triplet (Java / Sleela / C). It
implements the NS-SST-0001 format and reuses the shared Sleela front end, so it
enforces the same `#sleela` syntax-version rules. Reported by
`./build/nordshrift version`.

### NS-SST-0001 — `1.0.0` (Normative)
The normative specification of the `.sst` control-sheet format (`SST.model`).
The `#nordshrift 1.0` pragma inside a `.sst` file pins this format version.

### SL-META-0001 — `1.0.0` (Pre-Normative)
The Sleela Language Metadocument (`src/Sleela.manifest`) — the constitutional
document that governs the language's design, meta-model, and rules, including
the §4.4 syntax-versioning policy the compiler enforces. It is pre-normative
(revision 1.0.0) pending ratification by the Sleela Language Design Council.

## Versioning policy (from SL-META-0001 §8.3 and §4.4)

- **Semantic versioning** (`MAJOR.MINOR.PATCH`) applies to the language as a
  whole and to the implementation.
- **Syntax** is versioned independently as `MAJOR.MINOR`:
  - a **PATCH** increment introduces no grammar changes;
  - a **MINOR** increment may *add* constructs but must not remove or modify
    existing ones (backward compatible);
  - a **MAJOR** increment may introduce breaking grammar changes, with a
    minimum deprecation notice of two MINOR versions.
- A compiler **must reject** files whose declared syntax version exceeds its
  supported range. To widen support, raise `maxSupportedSyntax()` (and, when an
  old grammar is dropped, `minSupportedSyntax()`) in `impl/frontend/version.h`,
  then bump the toolchain version and record it here.

## How to bump a version

1. Change the source-of-truth location for the component (see the table).
2. Update the matching row (and any prose) in this file.
3. If the change affects `.sleela` acceptance, update
   `impl/frontend/version.h` and the `test-version` fixtures under
   `impl/tests/version/`.
4. Run `make test` and confirm `./build/sleela version` matches this file.

## History

| Date | Toolchain | Syntax | Notes |
|------|-----------|--------|-------|
| 2026-09 | 0.1.2 | 1.0 | Version-aware compiler added (enforces the `#sleela` pragma per SL-META-0001 §4.4); Nordshrift 1.0; NS-SST-0001 1.0.0; SL-META-0001 1.0.0 (pre-normative). |
