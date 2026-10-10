<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Documentation Index

This page is the entry point for contributors and operators. It distinguishes
normative specifications from implementation notes and live validation results.

## Language and compiler

- [Language syntax specification](../SLEELA.syntax) — checked-in grammar and
  documented syntax range. The current source snapshot documents **1.3–1.10**:
  the compiler version gate (`impl/frontend/version.{h,cpp}`) accepts 1.3–1.10
  and defaults to 1.10. The grammar defines the 1.8 features in §12 (inferred
  `let` locals and constructor arguments) — the fixtures in
  [`tests/sleela-syntax-1.8/`](../tests/sleela-syntax-1.8/) exercise them;
  the 1.9 GC-hint statement in §13; and the two meanings of `extends` in 1.10
  (OOD class inheritance and the document `extends to … <grouper>` form, see
  [`../markdown/EXTENDS.md`](../markdown/EXTENDS.md)). Constructor overload
  resolution and `this(...)`/base delegation are **not** part of verified 1.8
  and are rejected explicitly.
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
- [General implementation architecture](../markdown/ARCHITECTURE.md) — boundary
  between the authoritative C/C++ implementation and the earlier Java prototype.
- [Configuration guide](../config/CONFIGURATION.md) — configuration root and
  runtime/VM settings.

## Build, tests, and CI

- [Native implementation guide](../impl/README.md) — authoritative C/C++ build.
- [Testing guide](../markdown/TESTING.md) — local tests and translation-unit audit.
- [Test-suite guide](../test-suites/README.md) — broader suite entry points.
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