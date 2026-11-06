<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Local Generator

Scaffold a complete, local SLeeLa project **on disk** from a single high-level
**project intent**.

Where the [Website Generator](../website-generator) turns a design intent into a
self-contained *website*, the Local Generator turns a *project intent* into a
ready-to-run **local project layout**: a directory tree, starter `.sleela`
sources, a `Makefile`, a `README`, and a `.gitignore`, all consistent with the
house conventions of this repository.

## Architectural rule

> **SLeeLa decides the project; the host only writes the bytes.**

The generator is authoritative SLeeLa source under `generator/`. It owns the
mapping from a *blueprint* (name + kind + options) to an ordered list of
**artifacts** (relative path + file body). A thin host — a CLI, a `Makefile`, or
a GUI — materializes those artifacts to disk. The generator never shells out and
never touches the filesystem itself.

```text
   Project intent (name, kind, options)
                 |
                 v
        SLeeLa Local Generator               <-- authoritative
   (generator/*.sleela : blueprint + planner + emitter)
                 |
                 v
        Ordered list of artifacts
        (relative path + file body)
                 |
                 v
        Host writer -> project on disk
        (CLI / Makefile / GUI)
```

## Vocabulary

- **Blueprint** — the project intent: a `name`, a project `kind`
  (`app`, `library`, `service`, `cli`), and a few option dials
  (`license`, `withTests`, `withMakefile`, `sleelaVersion`).
- **Artifact** — one generated file: a repository-relative `path` and its full
  text `body`. A project is an ordered list of artifacts.
- **Plan** — the complete, ordered list of artifacts for a blueprint. The
  planner decides *which* files a project of a given kind needs; the emitter
  fills in each body.

## Project kinds

| Kind      | Produces                                                              |
|-----------|----------------------------------------------------------------------|
| `app`     | A runnable program with a `Main` class carrying `main()`.            |
| `library` | A reusable class plus a sibling demo that exercises it.              |
| `service` | A long-lived loop skeleton (serve/tick) with a `Main` entry.        |
| `cli`     | An argument-reading entry point with a usage banner.                |

Every kind also gets a `README.md`, a `.gitignore`, and — when `withMakefile`
is on — a `Makefile` that drives the SLeeLa runtime, exactly like the rest of
the repo.

## Layout

```text
local-generator/
  README.md                 This document.
  DESIGN.md                 The blueprint / artifact / plan model in detail.
  Makefile                  Build/preview helpers (dispatch into SLeeLa).
  generator/                SLeeLa generator (authoritative).
    generate.sleela         The whole generator: blueprint struct + planner +
                            emitter + a runnable main(), in one executive class.
  samples/                  Example project intents.
    starter.project         A sample blueprint (name + kind + options).
```

The generator is deliberately **one `.sleela` file with one class**. The SLeeLa
compiler recognizes the `meta*` fields of the Metadata Class metapattern and
auto-generates their accessors; two linked classes that both carry the
metapattern collide on those generated accessors, so the collaborating pieces
(blueprint, planner, emitter) are methods on a single executive class rather
than separate linked classes.

## Run the demo

```sh
cd local-generator
make generate      # runs the SLeeLa generator, prints the plan
```

`make generate` runs `generator/generate.sleela`, which composes a sample
blueprint and prints every artifact (its path, then its body) as a single plan
document. Each artifact is framed by a `>>> <path>` banner and a `<<< end
<path>` banner, so a host CLI can split the stream back into files. A host can
parse that plan and write each file to disk.

### Verified

Built against the source compiler (`impl/build/sleela`, syntax 2.1), `make
generate` prints a 5-artifact plan for the `library` preset, and each of the
four project kinds (`app`, `library`, `service`, `cli`) emits SLeeLa that
compiles and runs — e.g. the generated `library` project prints `Hello,
world!`.

## Status

The SLeeLa generator defines the full blueprint / plan / emit pipeline and a
runnable demo (`generator/generate.sleela`). Writing the plan to disk is the
host's job and is intentionally out of scope for the authoritative generator;
the `Makefile` prints the plan so the pipeline is verifiable end to end.

**Max Rupplin — MEARVK LLC — 2026**

---

# SLeeLa
<p align="center"><img src="../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
