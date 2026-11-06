# Local Generator — Blueprint & Artifact Model

This document is the contract between the SLeeLa generator and any host (a CLI,
the `Makefile`, or a GUI). It describes the single input — a **blueprint** — and
the output — an ordered list of **artifacts** that a host writes to disk.

## 1. Blueprint

A blueprint is the whole project intent. It is deliberately small and legible so
a host can collect it from a form or a few CLI flags.

| Field           | Type        | Meaning                                                      |
|-----------------|-------------|--------------------------------------------------------------|
| `name`          | string      | Project name, used for the root dir and in documentation.   |
| `cls`           | string      | Entry class name — a valid SLeeLa identifier, e.g. `SampleLib`. |
| `kind`          | enum int    | `0 app`, `1 library`, `2 service`, `3 cli`.                 |
| `license`       | string      | SPDX-style license id, e.g. `MIT`, `Apache-2.0`, `none`.    |
| `sleelaVersion` | string      | `#sleela` version stamped into generated sources.           |
| `withTests`     | int (0/1)   | Emit a sibling test/demo artifact.                          |
| `withMakefile`  | int (0/1)   | Emit a `Makefile` that drives the SLeeLa runtime.           |

`cls` is supplied explicitly rather than derived from `name`: the current
SLeeLa compiler exposes no primitive `String` length/index operation, so the
generator does not transform a dashed `name` into CamelCase on its own. A host
that wants automatic derivation computes `cls` and passes it in.

`kind` is a single dial that changes the shape of the whole project: the planner
chooses the file set, and the emitter chooses each body, from this one value.

## 2. Artifact

An artifact is one generated file.

| Field  | Type   | Meaning                                              |
|--------|--------|------------------------------------------------------|
| `path` | string | Project-relative path, e.g. `src/Main.sleela`.       |
| `body` | string | The full text of the file.                           |

A project is an ordered list of artifacts. Order matters only for human reading
of the printed plan; a host may write them in any order.

## 3. Plan

The planner maps a blueprint to the ordered artifact list:

- Always: `README.md`, `.gitignore`.
- When `withMakefile`: a `Makefile` driving `SLEELA_BIN run`.
- By kind:
  - `app` → `src/Main.sleela` (a `Main` class with `main()`).
  - `library` → `src/<Name>.sleela` + (when `withTests`) a sibling demo with `main()`.
  - `service` → `src/Main.sleela` with a serve/tick loop skeleton.
  - `cli` → `src/Main.sleela` reading args with a usage banner.

## 4. Host contract

A host composes a blueprint and asks the generator for the plan. In SLeeLa terms
(all methods on the single `Generate` class):

- `create(name, cls, kind, license, sleelaVersion, withTests, withMakefile)` → a `BlueprintModel`
- `preset(name)` → a ready blueprint for quick starts (`app` / `library` / `service` / `cli`)
- `buildPlan(blueprint)` → fills the ordered artifact arrays
- `renderPlan()` → the full plan as a single printable document

The generator never writes files. The host reads the printed plan (each artifact
is framed by a `>>> <path>` banner and a `<<< end <path>` banner) and
materializes it. The `Makefile` in this folder prints the plan so the pipeline
is verifiable without a host.

## 5. Conventions honored

Generated SLeeLa sources follow the repository's house style: a `#sleela`
version line, angle-bracket (`<` / `<<`) comments, and — for any non-trivial
generated class — the Metadata Class metapattern described in
`.kiro/steering/metadata-class-standard.md`.

**Max Rupplin — MEARVK LLC — 2026**
