# Nordshrift examples — what to learn here

Nordshrift is SLeeLa's **build-sheet** tool. A sheet (`.sst`) is a small
declarative file that says *what* to build: which sources to gather (by glob),
what to leave out (exclude), and which target language to produce. The tool then
`check`s or `build`s that set. These examples teach the sheet format from one
file up to glob-and-prune.

Read them in order. Each sheet is paired with the SLeeLa source it gathers.

## The three cases

| Level | Sheet | Paired source(s) | The one new idea |
|---|---|---|---|
| Basic | [`BASIC-single-source.sst`](BASIC-single-source.sst) | [`BASIC-Greeter.sleela`](BASIC-Greeter.sleela) | One glob, one file, one target. |
| Moderate | [`MODERATE-multi-source.sst`](MODERATE-multi-source.sst) | [`MODERATE-App.sleela`](MODERATE-App.sleela), [`MODERATE-Report.sleela`](MODERATE-Report.sleela) | One glob that gathers several self-contained programs. |
| Advanced | [`ADVANCED-exclude.sst`](ADVANCED-exclude.sst) | [`ADVANCED-Service.sleela`](ADVANCED-Service.sleela) (+ excluded [`ADVANCED-ServiceDraft.sleela`](ADVANCED-ServiceDraft.sleela)) | Collect broadly with a glob, then prune with an exclude pattern. |

The original [`CLASSIC-capstone.sst`](CLASSIC-capstone.sst) /
[`CLASSIC-Ledgerize.sleela`](CLASSIC-Ledgerize.sleela) pair is the first-read
starter.

## What we want you to take away

1. **A sheet is a plan, not code.** It has three blocks: `source:` (a `root` and
   a `glob`, optionally `exclude`), `target:` (a `root` and a `target-language`),
   and the `sheet name:` header with a `version` and `description`. Reading a
   sheet tells you the whole build at a glance.
2. **Globs gather; excludes prune.** The advanced sheet matches
   `ADVANCED-*.sleela` and then removes anything matching `*Draft*`, so a draft
   file can sit right next to the real one without being built. "Collect broadly,
   then exclude" beats renaming files or editing lists by hand.
3. **Each gathered source is its own unit.** Nordshrift validates every file in
   the set independently, so each source in a multi-source sheet is
   self-contained (its own `main`). That is why the moderate example gathers two
   complete little programs rather than an app plus a helper it imports.

## Running the tool

These sheets use the `#nordshrift 1.0` sheet dialect, which the bundled tool
accepts. From inside this folder:

```sh
# validate a sheet (prints diagnostics, builds nothing)
nordshrift check BASIC-single-source.sst

# build the sheet's sources to its target
nordshrift build MODERATE-multi-source.sst
```

A clean `check` reports `0 error(s), 0 warning(s)` and the resolved
`target-language`. The authoritative build-from-source path is
`make compiler` (see `working-binaries/README.md`); the targets a sheet can
produce are `sleela`, `java`, and `c`.

## A note on globs

A `glob` must match real filenames exactly. If a sheet reports "source file set
is empty after glob and exclude evaluation," the pattern matched nothing — check
the filename and the `root`. (The glob is literal about prefixes: a pattern like
`Ledgerize.sleela` will **not** match a file named `CLASSIC-Ledgerize.sleela`.)
