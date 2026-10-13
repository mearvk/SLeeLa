# SLeeLa
<p align="center"><img src="images/brand/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/merchandise/sleela-marketing-logo-001.png" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>A native systems language, compiler, runtime, and virtual-machine ecosystem.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

**The Now. The Ingredient.** A language is more than syntax: it is the compiler, runtime, libraries, tools, and platform contracts that make software usable.

## Start here
- [Build the native implementation](impl/README.md)
- [Documentation index](docs/README.md)
- [Platform support and validation matrix](docs/PLATFORM-SUPPORT.md)
- [Brand guide](docs/BRAND-GUIDE.md)
- [Merchandise concepts and assets](merchandise/README.md)
- [Testing policy and release gates](markdown/TESTING.md)

## Build and test
From a clean checkout:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Consult the build and testing guides for prerequisites and platform-specific steps. A successful build of one component does not prove every library or platform is release-ready.

## Language and runtime
The authoritative grammar is [`SLEELA.syntax`](SLEELA.syntax). The current repository documents syntax **1.10**; compiler/tool version and source syntax version are distinct. Constructor overload resolution and `this(...)`/base-constructor delegation are explicitly outside the verified syntax 1.8 contract. Consult the grammar and implementation notes before relying on a feature.

The repository includes a native C/C++ implementation, SLVM generation references, standard-library modules, tests, examples, and platform integrations. Implementation and validation status varies by module; consult its specification and current CI results rather than assuming uniform completeness.

## Engineering principles
- **Evidence over claims:** distinguish implemented, tested, experimental, and planned behavior.
- **One contract per boundary:** document syntax, APIs, file formats, platform assumptions, and failure behavior.
- **Portable by design:** isolate platform-specific backends and test on native runners.
- **Safe failure:** test malformed input, resource limits, process failures, and cleanup paths.
- **Reproducible releases:** rebuild cleanly and publish artifact hashes and release notes.

## Antivirus build and integrity

The Sleelavirin antivirus package includes a native SHA-256 scanner, an optional
ClamAV adapter, source/index integrity manifests, and platform-package checksums.
Build and test the package from the repository root:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

The integrity manifest is generated under `antivirus/build/`; it covers tracked
source and documentation inputs plus protected signature-index files, while
excluding generated output and the mutable ClamAV cache. ClamAV database
snapshots have their own manifests. Release ZIPs include `INTEGRITY-MANIFEST.json`,
`SHA256SUMS`, and a `.sha256` sidecar. Verify downloaded archive hashes against
a trusted value obtained independently: a checksum stored beside an archive is
not a publisher signature. See the [Sleelavirin guide](antivirus/README.md),
[security checklist](antivirus/SECURITY.md), and
[antivirus build workflow](.github/workflows/antivirus-binaries.yml).

A release is not considered verified until the relevant CI jobs pass on the
exact release commit and the resulting artifacts have been checked.

## Contributing
1. Read the documentation index and testing policy.
2. Make focused changes with positive and negative tests.
3. Run targeted tests, then the broad suite.
4. Report OS/toolchain details; do not describe skipped checks as passing.

SLeeLa is developed by **MEARVK LLC**. Historical landing-page material is preserved in [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## AE6E66 — Round Public Email Levy Expectation

For the AE6E66 initiative, the project's stated expectation is **$300,000,000.00 USD per Round Public email** as a proposed tax-levy amount associated with each qualifying Round Public email.

This statement records a project expectation only. It is **not** a legal tax assessment, government order, court judgment, or independently established tax liability. Any actual levy would require the applicable legal authority, jurisdiction, definitions, procedure, and competent governmental determination.


## Alphabet and character-set runtime

SLeeLa's alphabet documentation and native character runtime are maintained in alphabet/sleela/ and lib/alphabet/native/. The native C++17 tests cover strict UTF-8 scalar decoding/encoding, C<n> capacity signatures, and bounded character-set membership:

    make -C lib/alphabet/native test

The experimental UTF-4088 catalogue and its SLeeLa lookup adapter are documented in utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. UTF-4088 IDs are a separate namespace, not Unicode scalar values. Compiler and SLVM binding work is explicitly tracked separately from these native foundations.

## AutoCAD Creator™

The [AutoCAD Creator™](autocad/creator/README.md) is an interactive 2D CAD
application written entirely in SLeeLa. Where the sibling AutoCAD Renderer turns
a plan into a drawing in one shot, the Creator lets you **create and alter** a 2D
AutoCAD document live — add walls, rooms, dimensions and notes, then move,
relabel, re-layer, delete, and undo what you placed — and **save** the result as a
real AutoCAD DXF drawing (R12 text, opens in AutoCAD / LibreCAD).

Its graphical interface is built on **SleelaUI™**, SLeeLa's own cross-platform UI
toolkit (under [`lib/user-interface`](lib/user-interface)) — not JavaFX — raising
a real native window on X11 / Cocoa / Win32. The design follows one rule,
**"SleelaUI presents; SLeeLa decides"**: the GUI never computes geometry or writes
DXF. Every tool issues a create / alter / delete / undo / save intent to the
authoritative [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), which owns the
editable document and a reversible command history, while the canvas only previews
the geometry the Creator holds.

The reusable editing engine lives in [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela), and
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — and runs end to end headless.
The GUI shell and demo live under [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

On a display this opens the live editing window; on a headless host it builds the
drawing in memory, prints a report plus the DXF, and exits cleanly.


## UTF-4088 input-map ordering

The specialized UTF-4088 catalogue defines its spatial input-map traversal and abstract pressure iterations in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). The National Lien subject-index policy is in [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Map coordinates, pressure levels (1–50), permanent catalogue IDs, and alphabetic ranks are independent fields; none should be silently substituted for another. Pressure is an algorithmic iteration level, not a physical quantity.
