# NORDSHRIFT.md — the `.sst` transpiler driver

**Nordshrift** is the **transpiler driver for Sleela**. It reads a **`.sst`
Scripting Sheet** — the human-authored *control surface* defined by the
normative specification **NS-SST-0001** (`SST.model`) — and drives the
transpilation of the Sleela source files (**Wrapper™** files) the sheet names
into a selected target.

A `.sst` file is **not a program**: it is a build-control sheet. The program is
the set of `.sleela` sources its `source:` section points at (see
[`SOURCE.md`](SOURCE.md)). Nordshrift resolves those, runs them through the
shared Sleela front end, and emits the target the sheet selects.

```
   build.sst (control surface)      src/**/*.sleela — Wrapper™ files (the program)
        │                                     │
        ▼                                     ▼
   lex → parse → validate  ──►  resolve sources  ──►  Sleela front end (lex → parse → AST)
   (Sheet model + NSS-* diags)                              │
                                                            ▼
                                     target-language:  java | sleela | c   (the triplet)
                                     (javac+run)   (runs on C core)   (gcc+run)
```

## The triplet

The `target-language` directive selects one of three targets — **`java`**,
**`sleela`**, or **`c`** (the *triplet*; default `java`). For
`target-language sleela`, the emitted program is also executed on the Sleela
core, closing the loop.

## The sheet, in brief

A `.sst` sheet is **indentation-significant** and **pragma-first**: pragmas →
`sheet` block → optional `import`s → configuration sections.

```sst
#nordshrift 1.0            // required
#sleela     1.0            // optional: pins the Sleela syntax version

sheet demo:
  version      1.0.0
  description  "Transpiles the demo sources; the sleela target runs on the core."

source:
  root  "src"
  glob  "**/*.sleela"

target:
  root            "out"
  target-language sleela   // java | sleela | c
```

Sections: `sheet`, `import`, `source`, `target`, `pipeline`, `rules`, `effects`,
`derive`, `guards`, `interop`, `profile`. Diagnostics are emitted as
`NSS-{E|W|N}-{XXXX}`, each with file, line, message, and governing rule.

## CLI

```sh
nordshrift check <sheet.sst>    # lex + parse + validate; print all NSS-* diagnostics
nordshrift build <sheet.sst>    # resolve sources, transpile to target-language;
                                #   the sleela target additionally runs on the C core
nordshrift version
```

## Authoritative references

This file is a concise overview. The definitive detail lives in:

- **`SST.model`** — NS-SST-0001, the normative `.sst` format (grammar, section
  schemas, the full diagnostic index).
- **`impl/nordshrift/NORDSHRIFT.md`** — the implementation guide: exact section
  status (honored / validated / parsed), the implemented diagnostic set, and the
  module layout.

Nordshrift reuses the shared Sleela front end, so it enforces the same `#sleela`
syntax-version rules as the compiler ([`COMPILER.md`](COMPILER.md) §3).
