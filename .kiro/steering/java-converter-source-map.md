---
inclusion: fileMatch
fileMatchPattern: ['sleelactl/convert/**', 'lib/java/**', 'java28/**', 'impl/subjects/conversion/**', 'examples/java/**', 'docs/reference/JAVA.CONVERSION.STATUS.md']
---

# Java to SLeeLa converter: where the source lives

Recorded 2026-11-19 from master `630121e5`. Registered in `bin/sleelactl.d/registry.conf`
as `java-convert` (nature=tool, dir=sleelactl/convert, bin=javaconv).

## Source locations

| Path | What it is |
|---|---|
| `sleelactl/convert/` | C++ converter: `java_lex.c/.h` (C99 tokenizer), `conv_model.c/.h` (model loader), `java_to_sleela.cpp/.h` (translator), `javaconv_main.cpp` (CLI), `Makefile`, `verify-provenance.py`, `README.md`, `tests/java/` (49 fixtures) |
| `impl/subjects/conversion/` | `conversion.model` (shared model) and `CONVERTER-IDEMPOTENCE.md` |
| `lib/java/` | SLeeLa-native converter and Java library; spec in `lib/java/JAVA.SPEC.md` |
| `examples/java/` | Differential examples |
| `tests/java_constraints/`, `tests/java_equivalence/` | Constraint and equivalence tests |
| `java28/` | JDK 28 signature and reflection scaffolding |
| `impl/backup/java-converter-2026-11-13/` | Backup of compiler.cpp and parser.cpp from 2026-11-13 |
| `sleelactl/*.SESSION.*.md` | Session records |
| `docs/reference/` | `JAVA.CONVERSION.STATUS.md`, `SESSION.CONTINUATION.2026-11-18.md`, `CONVERTER.MAIN-WORK.PROGRESS.2026-11-18.md`, `PRODUCTION.RELEASE.QUALIFICATION.2026-11-18.md` |
| `.github/workflows/java-convert.yml`, `sleelactl.yml` | CI |

## Status at time of note

- Release decision: HOLD. No current-master build or differential run is recorded.
- Last measured: 2026-11-13, 32/32 differential checks; JDK 25 whole-tree 420/884 files converted.
- Open: PR #378 (workflow-only validation campaign). Many `work/java-*` branches exist on the remote;
  they were not compared against master for unmerged changes.
- The C++ converter fails closed per file. The SLeeLa-native converter emits per-member stubs.
  Count refusals, stubs and implemented members separately.
