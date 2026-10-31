<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Documentation Index

This page is the entry point for contributors and operators. It distinguishes
normative specifications from implementation notes and live validation results.

## Language and compiler

- [Language syntax specification](../SLEELA.syntax) — normative grammar for
  the documented **1.3–2.9** source-syntax range. The compiler gate in
  `impl/frontend/version.{h,cpp}` accepts syntax **1.0–2.9** and defaults
  to its maximum; individual features remain subject to their own version
  gates. The current edition adds `@mix` consequent fields (§28); the
  `prestige` marker in §27 is explicitly future-facing and is not an
  implemented feature. The compiler/tool release number is independent of the
  source syntax edition. See the [syntax version matrix](../api/sleela/2.1/language/SYNTAX-VERSION-MATRIX.md)
  for feature history and implementation boundaries. Constructor overload
  resolution and `this(...)`/base delegation remain outside the verified
  implementation and are rejected explicitly.
- [Compiler documentation](../markdown/COMPILER.md) — pipeline and version-aware
  behavior.
- [Source and file format](../markdown/SOURCE.md) — `.sleela` source contract.
- [Library symbol inventory](../lib/LIBRARY.SYMBOLS.md) — canonical library
  vocabulary.

## Architecture and virtual machines

- [SLVM/6 architecture](ARCHITECTURE.md) — architecture, implementation
  boundaries, migration, and resolver lineage.
- [SLVM/6 security](SECURITY.md) — intended controls and evidence needed to
  verify them.
- [Virtual-machine generations](../sleela-virtual-machine/) — generation-specific
  specifications and implementation references. A specification is not proof
  of complete runtime support.
- [Virtual machines & Modium — hands-on user guide](../virtual-machine/USER-GUIDE.md) —
  try-it guide for the `sleela vm` selector (Native VM is the default) and the
  `sleela modium` file-resolution scan (SHA-256 + per-version `rebuild.sql`).
- [General implementation architecture](../markdown/ARCHITECTURE.md) — boundary
  between the authoritative C/C++ implementation and the earlier Java prototype.
- [Registry](../registry/README.md) — Windows-style hierarchical registry
  (settings, feature flags, per-artifact provenance + originality seal); the
  master build materializes its baseline table (text store + MySQL export),
  reviewable with `sleela registry`. Sleela OOD mirror in
  [`lib/registry`](../lib/registry/README.md).
- [CPU stack](../lib/cpu/CPU.md) — a complete CPU/OS/program machine in SLeeLa
  source (gates → ALU → cores → OS → programs) plus a roster of 130+ CPU
  profiles, each with per-architecture docs. Shared, adjustable timing is
  documented in
  [Voltages, Variable Clocks, and Expert Timings](../lib/cpu/TIMING.md)
  (`SLVoltageRail`, `SLCpuTiming`, `SLBusTiming`, `SLRamTiming`, `SLIOTiming`,
  the Novice/Basic/Advanced/Expert tiers, and the `SLTimingManifest` a compiled
  `.sleela` image carries), and running a CPU at spec / overclocking it plus the
  RAM / storage / bus parts bin and DMA / GPU / native-RAM options are in the
  [executor components guide](../lib/cpu/components/README.md).
- [Configuration guide](../config/CONFIGURATION.md) — configuration root and
  runtime/VM settings.

## Build, tests, and CI

- [Native implementation guide](../impl/README.md) — authoritative C/C++ build.
- [Testing guide](../markdown/TESTING.md) — local tests and translation-unit audit.
- [Test-suite guide](../test-suites/README.md) — broader suite entry points.
- [Sleelavirin antivirus build and integrity](../antivirus/README.md) — native scanner, optional ClamAV integration, SHA-256 source/index manifest, and release package checksums.
- [Antivirus security and release checklist](../antivirus/SECURITY.md) — index provenance, database snapshot integrity, safe scan behavior, and release gates.
- [Antivirus binary build workflow](../.github/workflows/antivirus-binaries.yml) — supported build matrix and release artifact packaging.

From the repository root, run `make antivirus`, `make antivirus-test`,
`make antivirus-integrity`, and
`make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json`.
These checks document the build and integrity process; they do not replace
review of the exact commit's CI results or independently trusted release hashes.

The repo-root `make all` finishes by building the **registry** (the `registry`
target runs [`build-registry.sh`](../build-registry.sh)), which materializes the
baseline registry table — the portable text store `registry/build/sleela.sleelareg`
and the MySQL script `registry/build/registry.sql`. Build it on its own with
`make registry` and review it with `sleela registry` or
`registry/build/sleela-registry review`.

- [GitHub Actions](https://github.com/mearvk/SLeeLa/actions) — current run results.

**Status note:** the latest inspected Linux and Windows subject-test runs reported
invalid-pointer crashes in the physics, economics, inference, and finance
fixtures. This documentation update does not fix those runtime failures. Consult
the live CI results and do not describe the full suite as green until a later run
passes.

## Documentation maintenance rules

1. Update normative specs, implementation, and regression tests together when
   changing language behavior or supported syntax.
2. Label design intent separately from behavior verified in the current build.
3. Link security claims to the enforcing code and tests; distinguish technical
   controls from external certification.
4. Report test results with the run/commit context and link to CI instead of
   implying a permanent pass from historical successes.
5. Keep this index current when adding, renaming, or retiring core documentation.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
