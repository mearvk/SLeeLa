# Nordshrift — the `.sst` transpiler driver (NS-SST-0001)

Nordshrift is the **transpiler driver for Sleela**. It reads a **`.sst` Scripting
Sheet** — the human-authored *control surface* defined by the normative
specification **NS-SST-0001** (`/SST.model` in this repo) — and uses it to drive
transpilation of the Sleela source files the sheet names.

The `.sst` file is **not a program**: it is a build-control sheet. The program
is the set of `.sleela` source files — each a **Wrapper™** (the `.sleela` file
type: a Sleela source file carrying the metadocument addend, governed by
SL-META-0001) — that the sheet's `source:` section points at. Nordshrift
resolves those, runs them through the shared Sleela front end, and emits the
target the sheet selects.

```
   build.sst (control sheet)          src/**/*.sleela — Wrapper™ files (the program)
        │                                     │
        ▼                                     │
   lex → parse → validate  ── source: glob ──▶ resolve file set
   (Sheet model + NSS-* diagnostics)          │
        │                                     ▼
        │                        Sleela front end (lex → parse → AST)
        │                                     │
        └────────── target-language ─────────┤  triplet emitter
                                              │
                       ┌──────────────────────┼──────────────────────┐
                       ▼                       ▼                      ▼
                     java                   sleela                    c
                 (javac + run)        (runs on the C core)       (gcc + run)
```

## Relationship to the spec

`/SST.model` (NS-SST-0001, Revision 1.0.0) is authoritative. This implementation
follows it for the lexical layer, file structure, the section schemas, and the
diagnostic code system. Two deliberate, conformant deviations/extensions:

1. **Triplet target.** The spec's `target` section is Java-only. Nordshrift
   drives a *triplet*, so `target` accepts an additional directive
   **`target-language`** (`java` | `sleela` | `c`, default `java`). This is a
   superset: a spec-conformant sheet with no `target-language` behaves exactly
   as the spec describes (Java).
2. **Deferred semantics.** The `rules`, `effects`, `derive`, `guards`,
   `interop`, and `profile` sections are fully **lexed, parsed, and validated**
   (with their NSS-* diagnostics), but are **not yet applied** to emission in
   this pass. They are recorded in the sheet model for the next stage. The
   `sheet`, `import`, `source`, `target`, and `pipeline` sections are honored.

The companion `SL-META-0001` (the Sleela Language Metadocument the spec refers
to for Entity/Contract/Effect/Rule/Lens/Flow) is not present in the repo; the
meta-model concepts those deferred sections reference are stubbed accordingly.

## The `.sst` sheet

A sheet is **indentation-significant** (2- or 4-space unit, fixed by the first
indent) and **pragma-first**. Structure: pragmas → `sheet` block → optional
`import`s → configuration sections.

```sst
#nordshrift 1.0            // required (NSS-E-0003 if missing)
#sleela     1.0            // optional

/// A documentation comment attaches to the block it precedes.
sheet demo:
  version      1.0.0
  author       "Sleela Design Council"
  description  "Transpiles the demo sources; the sleela target runs on the core."

source:
  root  "src"
  glob  "**/*.sleela"      // recursive glob; ** crosses directories

target:
  root            "out"
  layout          mirror-source
  java-version    21       // must be >= 17 (NSS-E-0040)
  package-root    "com.example.demo"
  target-language sleela   // triplet selector: java | sleela | c
```

### Sections (per the spec)

| Section    | Purpose                                                          | Status here |
|------------|------------------------------------------------------------------|-------------|
| `sheet`    | file manifest: version, author, description, tags, extends       | honored     |
| `import`   | compose sheets (`as`, `only`, `except`)                          | parsed      |
| `source`   | root + globs − exclude; encoding, watch                          | honored     |
| `target`   | root, layout, java-version, package-root, +`target-language`     | honored     |
| `pipeline` | phases, skip, parallel-threshold, cache, verbosity, fail-fast    | validated   |
| `rules`    | activate/deactivate, severity, config                            | parsed      |
| `effects`  | policy, declare, aliases, default-effect                         | parsed      |
| `derive`   | lens/projection/equality/…, target-style                        | parsed      |
| `guards`   | mode, on-failure, message-format                                 | parsed      |
| `interop`  | assume-impure, null-wrapping, checked-exceptions, type-mapping   | parsed      |
| `profile`  | named variant with `inherits` + overriding sections              | parsed      |

## CLI

```sh
nordshrift check <sheet.sst>    # lex + parse + validate; print all NSS-* diagnostics
nordshrift build <sheet.sst>    # resolve source: files, transpile to target-language;
                                #   the sleela target additionally runs on the C core
nordshrift version
```

`build` resolves `source.root`/globs relative to the sheet's own directory. Each
resolved `.sleela` file is parsed by the shared Sleela front end and emitted in
the selected language. For `target-language sleela`, the emitted program is also
executed on the Sleela core, closing the loop.

## Diagnostics (NS-SST-0001 Part XV)

Every diagnostic is `NSS-{E|W|N}-{XXXX}` with the file, line, a concrete
message, and the governing rule. Implemented so far:

| Code | Condition |
|------|-----------|
| NSS-E-0001 | invalid UTF-8 |
| NSS-E-0002 | mixed / inconsistent indentation (IND-01/03) |
| NSS-E-0003 | missing `#nordshrift` pragma |
| NSS-E-0004 | unsupported `#nordshrift` version |
| NSS-E-0010 | multiple `sheet` blocks |
| NSS-E-0021 | duplicate import alias |
| NSS-E-0030 | `source.root` does not exist |
| NSS-E-0031 | empty source set after glob/exclude |
| NSS-E-0032 | absolute path in `source.glob` |
| NSS-E-0040 | `target.java-version` < 17 |
| NSS-E-0050 | pipeline phases out of order |
| NSS-E-0051 | skipping a mandatory phase |
| NSS-E-0080 | `derive builder true` with `target-style record` |
| NSS-E-0110 | multiple-parent profile inheritance |
| NSS-W-0001 | orphaned documentation comment |
| NSS-W-0100 | `null-wrapping trust` activated |

## Files

```
nordshrift/
  NORDSHRIFT.md         this document
  README.md             quick start
  diagnostics.h         NSS-* diagnostic model (Part XV)
  sst_lexer.{h,cpp}     indentation-significant tokenizer (Part I) -> INDENT/DEDENT
  sheet_model.h         the Sheet model (all sections, §II–§XIII)
  sst_parser.{h,cpp}    recursive-descent parser over INDENT/DEDENT (Part XIV EBNF)
  source_resolve.{h,cpp} filesystem glob resolution of the source section (§V)
  sleela_emit.{h,cpp}   Sleela AST -> java | sleela | c  (the triplet emitter)
  nordshrift.cpp        the `nordshrift` CLI (check / build)
  examples/
    commerce-engine.sst the spec's full annotated example (validates clean)
    demo/build.sst      a minimal, self-contained sheet
    demo/src/Demo.sleela the program it transpiles
```

See `/SST.model` for the complete normative grammar (Part XIV) and the full
diagnostic index (Part XV §15.2).
