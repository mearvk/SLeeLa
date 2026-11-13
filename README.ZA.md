<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — South Africa (ZA)
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>A native systems language, compiler, runtime, and virtual-machine ecosystem.</strong></p>

<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **South African country edition.** This file provides the complete English technical edition (sections I–XIX, mirroring the canonical README), the short introductions in the South African languages listed below, and the retained Afrikaans technical guide. Full reviewed translations of the technical reference into the other listed languages are not yet available; the English edition is authoritative. Technical identifiers, source code, commands, filenames, links and API names are unchanged.

## Language editions

This country edition groups English, isiZulu, isiXhosa, Afrikaans, Sepedi, Setswana, Sesotho, siSwati, Tshivenda, Xitsonga, isiNdebele, and South African Sign Language. The introductory descriptions below are retained; the practical technical guide is provided in English pending reviewed translations in the other languages.

SLeeLa is a native systems programming language, compiler, runtime, and virtual-machine ecosystem. It brings together the language, compiler, libraries, developer tools, and platform contracts needed to build and run software.

- isiZulu: I-SLeeLa iwulimi lokuhlela amasistimu.
- isiXhosa: I-SLeeLa lulwimi lwenkqubo yeenkqubo.
- Afrikaans: SLeeLa is ’n inheemse stelselprogrammeertaal.
- Sepedi: SLeeLa ke leleme la tlhago la go rulaganya ditshepedišo.
- Setswana: SLeeLa ke puo ya tlhago ya go kwala mananeo a ditsamaiso.
- Sesotho: SLeeLa ke puo ea tlhaho ea mananeo a litsamaiso.
- siSwati: I-SLeeLa lulwimi lwemvelo lwekuhlela tinhlelo.
- Tshivenda: SLeeLa ndi luambo lwa u dzudzanya sisiteme.
- Xitsonga: SLeeLa i ririmi ra minongonoko ya tisisiteme.
- isiNdebele: I-SLeeLa lilimi lemvelo lokuhlela amasistimu.

---

**The Now. The Ingredient.** A language is more than syntax: it is the compiler, runtime, libraries, tools, and platform contracts that make software usable.

**What SLeeLa is involved in.** SLeeLa is involved in **Math**, **Object**,
**Science**, and **Reach** — the four faces of its work: the calculable
(Math), the settled thing standing in the catalogue of what is (Object), the
disciplined study of the true (Science), and the extension of a system's hand
to other systems and people (Reach). **National Interests remain** — they are
not set aside by any of the four; where a subject bears on a nation's interest,
that standing is kept (recorded faithfully, e.g. in each class's Metadata
`metaOrigin` / `metaNationalSchedule`), never dissolved.

## I. Start Here
- [Build the native implementation](impl/README.md)
- [Virtual machines & Modium — hands-on user guide](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU stack in SLeeLa source](lib/cpu/CPU.md) — a full gates→ALU→cores→OS→programs machine, a roster of 130+ CPU profiles, and adjustable [voltages, variable clocks, and expert timings](lib/cpu/TIMING.md) you can [run at spec and overclock](lib/cpu/components/README.md)
- [Documentation index](docs/README.md)
- [CSPML data source and Salt & Salary research reference](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — external U.S. place, socioeconomic, workforce, education, and public-school data provenance and integration guidance.
- [Generated API documentation](docs/api/README.md) — current HTML reference, versioned API edition, and regeneration instructions
- [API catalogue](api/README.md)
- [Computational comparison standards](comparison/README.md) — SLeeLa VM, Java/JVM, and native C/C++ type, memory, ABI, and conformance standards.
- [Recursion stanchion — C and C++ prototypes](recursion/stanchion/README.md) — primitive-specialized examples for Boolean, Byte, Character, Integer, Long, Float, and Double, alongside generic, applied, scientific, and teaching recursion patterns.
- [Platform support and validation matrix](docs/PLATFORM-SUPPORT.md)
- [Brand guide](docs/BRAND-GUIDE.md)
- [Merchandise concepts and assets](merchandise/README.md)
- [NitroWebExpress logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testing policy and release gates](markdown/TESTING.md)

## II. News

Recent additions to the language, drivers, and standard library:

- **The `advantage` standard-library package — advantaging a dividend, with
  math, physics, integration, grading, and routable output.**
  [`lib/advantage/`](lib/advantage/README.md) encapsulates *advantaging a
  unique dividend with respect to a model* as a layered **Sequitur onion**
  (dividend → review → concerns → maths → additions → subtraction → regrouping
  → iteration), navigable inward/outward with full dynamic update and
  restitute, exposed through a simple **user's model** and a full-control
  **developer's model**. It carries five subject areas: **`math/`** bilateral
  trade economics (joint profit maximization, Nash bargaining, the bilateral
  newsvendor critical ratio), **`physics/`** a structural-economics "physics of
  trust fields" (trust as a metric tensor, the transactional geodesic, an
  announcement contact structure, a CIR+jump trust-decay SDE — a modelling
  analogy, flagged as such), **`integration/`** a recursively-composable
  vertical-integration model for capitalism/retail (compartments, engineering
  bays, trial compartments, newton median trays, and a system-sophistication
  score), **`grading/`** a sophistication circuit grader that rates each module
  and any set of winning/losing/grader/absolute modules A–D, and **`output/`**
  a routable emit seam that sends module output to the terminal, a
  **CommonRails**-formatted surface, or a socket/connector (framed for a native
  `slnet_write`). Every class carries the Metadata-Class record; five runnable
  demos validate the subjects, and the companion
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md), and
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) document them. A native C/C++
  consumer under [`advantage/`](advantage/README.md) (the C core
  `advantage_value`/`advantage_mean`, an XML knowledge store, a secure
  HTTPS/SFTP/SSH acquisition + keystore + DH/ECDH transport library) links the
  model into worked
  [trade examples](advantage/examples/README.md) — positive trades from
  presumed internet sources, an audience-relevance sequence (Traders,
  Consumers, Helpers, Healers), and eight models ranked A–D (D = most Cement,
  A = most Brand).
- **Loop-timing annotations — `@loopDelay` / `@loopBudget` (+ a `sleep(ms)`
  built-in).** The first **statement-level, behavioral** annotations in SLeeLa:
  written on their own line before a `for` / `while` / `do` loop, they insert a
  real pause into the loop's execution. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` sleeps a fixed amount per iteration (a bare
  `@loopDelay(250)` defaults to milliseconds); `@loopBudget(seconds = 10,
  iterations = 1000)` spreads a known iteration count evenly over a total
  wall-clock budget. Both lower to the new append-only opcode **`OP_SLEEP`**
  (ISA 157), which also backs a general **`sleep(<ms>)`** built-in callable
  anywhere; the pause is implemented portably (`nanosleep` / `Sleep`). Specified
  in [`SLEELA.syntax` §32](SLEELA.syntax); API reference in
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  runnable
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  A session-additions index (including this, the native `OP_BIGMUL`
  big-integer multiply, and the standalone **[local-generator](local-generator/README.md)**
  and **[pi-generator](pi-generator/README.md)** tools) lives at
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  The three generated API surfaces
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md), and the immutable
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)
  snapshot) were rebuilt with `make api-docs` and now carry the new `SLISASleep`
  / `SLISABigMul` instruction entries on their `vm` package page.
- **SleelaDoc from the command line — `sleela docs`.** SleelaDoc is now reachable
  straight from the terminal. `sleela docs` lists every documentable topic;
  `sleela docs crypto` shows a package; `sleela docs stringbuilder` resolves a
  class (case-insensitively, and by the `SL` standard-library prefix →
  `SLStringBuilder`) and prints a rich page — **definition, design purpose,
  relationships, structure, user clays, and a five-rung mastery ladder** — with
  ranked "did you mean" results for a near miss. Authors enrich any page simply
  by adding SleelaDoc `@`-tags (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). `sleela docs --generate [dir]` writes a plain-text corpus
  (one page per class + a `TOPICS.index`) that the CLI then reads offline;
  **`make sleeladoc`** bundles that corpus **and the matching `.sleela` sources**
  into `sleeladoc/build/sleeladoc.jar`. For facade-only subjects (e.g.
  `psychiatry`, `politico`), `sleela docs --facade <topic>` searches the
  filesystem at runtime and lists the real **folders (then files, as natural
  motion)**, bounded by user-overridable caps of **6 MB / 6000 files / 6
  minutes**. The `lib/sleeladoc` units are maintained at the current edition,
  **`#sleela 3.0`**. See [`man/man1/sleela.1`](man/man1/sleela.1) and
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — a Javadoc-style documentation solution, with a generated HTML
  site.** **SleelaDoc** is SLeeLa's in-source doc-comment convention
  (angle-bracket comments sentinelled with `@doc` / `@page`; see
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), and
  [`lib/sleeladoc`](lib/sleeladoc/README.md) is the **extractor** that renders a
  `.sleela` unit as a Markdown or HTML API page — the way `javadoc` renders Java,
  documenting *intent*. A complete, browsable **HTML documentation site for the
  entire `/lib`** is generated into [`/sleeladoc-api`](sleeladoc-api/README.md)
  by `tools/generate-sleeladoc-api.py` (open `sleeladoc-api/index.html`; a clean
  blue / light-blue / light-grey / white style with a package → class → member
  hierarchy and a glossary written for the *Student 2* reader). The compiler also
  emits author-marked doc outputs as a byproduct of compilation (`--wiki`
  MediaWiki page + an originated source twin), documented in
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Runnable:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** A SLeeLa-side XML DOM — a document
  owning a tree of element/text/comment/CDATA/PI nodes with ordered children and
  named attributes, a serializer (compact/pretty, XML 1.0 §2.4 escaping), a
  char-cell parser, and the `SLXmlAspect` quality register. Node equality is a
  three-rung tie of trust (identity → same subject → structure), reflexive,
  symmetric, and deterministic. See [`lib/xml/README.md`](lib/xml/README.md) and
  the runnable [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Wide signed integer primitives (`long`, `int64`, `int128`).** `int`, `long`,
  and `int64` are all 64-bit signed integers that type-check and run identically;
  `int128` is accepted as an integer type (64-bit-backed for now, with true
  128-bit a documented follow-up). The base-2 signed series `int32`..`int16777216`
  (syntax 2.7) and the matching unsigned series `uint32`..`uint16777216`
  (syntax 2.8) extend this. See [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **`@mix` consequent fields (syntax 2.9).** A class may declare multiple
  instance fields with the **same name** when each repeat carries `@mix`; the
  language understands the group as consequent (an array / list keyed by index),
  top-down `1,2,3,…` by default or manual via `@mix(1,2,3,...)` whose index
  count must match the number of fields. See [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Document markers — the Natural Process, titled the Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  These bare document-marker words are collectively titled the **Preonems**
  (one marker is a *Preonem*; the set, the *Preonems* — see
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). First-class, declarative-only
  document markers naming what a document *is* in the order of making:
  `author`/`authorial` (syntax 2.4; handled by professional council — the one
  member with a behavioral effect), `material;` (settled Source) and `memory;`
  (syntax 2.5), `determine;` (syntax 2.6; a document open to be processed
  further by a doctor, physician, or ghost toward completion by legal hand),
  and the two newest, **`object;`** (a settled thing standing in the catalogue
  of what is — **reified, given concrete standing, at syntax 2.8**) and
  **`field;`** (the ground of relation in which objects stand and move) —
  both gated from syntax 2.6. Each is bare, contextual, and version-gated by the
  front end with no effect on parsing, typing, or codegen; outside the marker
  position every one stays an ordinary identifier (so `field` never disturbs a
  class member or a `.field` access). See [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23, and §26.
- **Nordshrift at syntax 3.0 with full library reach.** Nordshrift resolves
  through the same recursive `/lib` index as the compiler, reaching every
  package and `.sleela` unit; `nordshrift check` reports the whole-tree counts.
- **Recursion stanchion.** The [`recursion/stanchion/`](recursion/stanchion/README.md) directory now includes dedicated C and C++ reference prototypes for the seven documented primitive contracts, plus generic, applied, scientific, and teaching examples. These are reference sources; compile and test them before treating them as verified.

- **Angle-bracket comments (`<` / `<<`).** A new go-forward comment style —
  `< line >` and `<< block >>` — lexed by the SLeeLa frontend and documented in
  [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Radix integer literals.** `int` literals now accept hexadecimal (`0x`),
  binary (`0b`), and octal (`0o`) forms with `_` digit separators, in both the
  SLeeLa source language ([`SLEELA.syntax`](SLEELA.syntax) §1.4) and the
  [`.xlt` CPU-model format](lib/xlt/README.md).
- **The Metadata Class standard across the library.** The descriptive-metadata
  metapattern (provenance, legality, jurisdictional scope) is now carried by
  every class-bearing `.sleela` file under `/lib`; see
  [`SLEELA.syntax`](SLEELA.syntax) §15 and
  [the tutorial lesson](tutorial/13-metadata-class.md).
- **A richer text / String family.** `lib/text` gains real in-language helpers,
  a char-cell view giving genuine `charAt`/`substring`/`indexOf`/`reverse`/
  column-alignment, and a real positional templater — see the
  [text & String tutorial](tutorial/14-text-and-strings.md) and the runnable
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **First-class `String` in Nordshrift sheets.** A `.sst` sheet can declare a
  typed `strings:` block that contributes to the build's component manifest;
  see the [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md).
- **CPU models run by both opcodes and function-call equivalence.** Every
  `SL<ARCH>CPU` inherits the basis of running a native instruction stream
  through its fetch-decode-execute loop *and* running the code of an assumed
  other process by equivalent calls on the shared VM substrate — see
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Build and Test

**Prerequisites:** a C/C++ toolchain (GCC or Clang) and GNU Make; Python 3 for
the manifest/flow tooling; on Linux the X11/Xrandr development packages for the
display products (a module's `make deps` reports/installs what it needs). The
`impl/` build is fail-closed on a trusted SHA-256 manifest, which the root
`Makefile` points at automatically.

From a clean checkout, the quickest start:

```sh
make                                  # build everything from the repo root
```

Or build and test just the native core:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

The repo-root `make` builds every product (core, VM, codecs, churn,
antivirus, …) and ends by building the **registry** and materializing its
baseline table — so a built tree ships a reviewable registry out of the box.
The essential targets:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Other useful targets:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

The `registry` target runs [`build-registry.sh`](build-registry.sh): it builds
the registry core, seeds the baseline table into the portable text store
`registry/build/sleela.sleelareg`, and exports the MySQL table script
`registry/build/registry.sql` (via `sleela registry export`). Review it with
`sleela registry` or `registry/build/sleela-registry review`. See the
[registry guide](registry/README.md) and [Registry 3.0 architecture](registry/REGISTRY-3.0-ARCHITECTURE.md).

Consult the build and testing guides for prerequisites and platform-specific steps. A successful build of one component does not prove every library or platform is release-ready.

## IV. Install

After building the native products, install the commands and runtime support files:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

The conventional installer uses `/usr/local/bin`, `/usr/local/lib/sleela`,
and `/usr/local/share/sleela` by default. It preserves an existing startup
configuration and refuses to overwrite untracked existing commands. Uninstall
removes only files recorded by the installer, preserving configuration and
untracked runtime data.

For a focused, guided deployment of just the main package — the `sleela`,
`sleelvac`, and `nordshrift` binaries, the standard library, protocols, and
support modules — use the clean installer:

```sh
make clean-install       # guided install of the main package only
```

It makes one primary decision (install the main package), then a scope choice,
then on success an optional picker for curated enhancement modules. For the
interactive deployment workflow and all installation options, see
[Quick and Safe Install](install/README.md).

## V. Language and Runtime
The authoritative grammar is [`SLEELA.syntax`](SLEELA.syntax). The current repository documents syntax **3.0**; compiler/tool version and source syntax version are distinct. The 3.0 surface adds the **`pure` and `gasoline` document markers** (§30) — a pair of bare, contextual, declarative-only Preonems that mean nearly the same thing, each marking the document’s standing by its own honesty. Syntax 3.0 consolidates the 1.0–2.9 language surface into the current normative edition, preserving all feature-specific minimum gates. The prior 2.9 surface adds **`@mix` consequent fields** (§28) — a class may declare multiple same-named instance fields when each carries `@mix`, understood as one indexed group (top-down `1,2,3,…` or manual `@mix(1,2,3,...)` whose index count must match the group). It carries forward the base-2 signed/unsigned wide-integer families `int32`..`int16777216` (§24) and `uint32`..`uint16777216` (§25) over the earlier `long`/`int64`/`int128` (§21); **keyword supporter documents** (§22); and the **Preonems** — the declarative-only document markers `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30), and `artifact` (§35, legislature-grade standing graded unto time and space), collectively titled the Preonems (§26.1). Constructor overload resolution and `this(...)`/base-constructor delegation are explicitly outside the verified syntax 1.8 contract. Consult the grammar and implementation notes before relying on a feature. SleelaDoc's nine source units now declare `#sleela 3.0`; the shared compiler gate accepts source syntax 1.0–3.0. The new API build is produced by `make api-docs`.

The repository includes a native C/C++ implementation, SLVM generation references, standard-library modules, tests, examples, and platform integrations. Implementation and validation status varies by module; consult its specification and current CI results rather than assuming uniform completeness.

## VI. Engineering Principles
- **Evidence over claims:** distinguish implemented, tested, experimental, and planned behavior.
- **One contract per boundary:** document syntax, APIs, file formats, platform assumptions, and failure behavior.
- **Portable by design:** isolate platform-specific backends and test on native runners.
- **Safe failure:** test malformed input, resource limits, process failures, and cleanup paths.
- **Reproducible releases:** rebuild cleanly and publish artifact hashes and release notes.

## VII. Antivirus Build and Integrity

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


## VIII. SLeeLa Editor Themes — Vim and Emacs

SLeeLa includes a coordinated set of professional dark color schemes for **Vim** and **Emacs**, plus a native C/C++ palette utility. The original **American Professional** scheme remains the default. Ten companion palettes are available for different cultural and institutional design inspirations; these are interpretive design palettes, not official representations of any nation, culture, court, or institution.

### The Eleven Schemes and Their Colors

| Scheme | Background | Foreground | Keywords | Types | Strings | Comments |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russian and Slavic — Medium** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Central Asian / Silk Road** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **East Asian** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — International Criminal Court** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Greek (Hellenic)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Turkish (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

#### National flag colours (source palette)

The Greek and Turkish schemes are derived from the two countries' **official national flag colours**, shown here with their exact published hex values. The scheme backgrounds/accents above are darkened, editor-friendly interpretations of these source colours — the table below is the "colors as nationally exposed" reference.

| Nation | Colour | Hex | Basis and meaning |
|---|---|---|---|
| **Greece** | Blue | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | National flag blue (Law 851/1978). The sky and the seas; blazoned heraldically as *azure*. |
| **Greece** | White | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | National flag white. Purity of the independence struggle; the cross denotes Eastern Orthodox Christianity. |
| **Türkiye** | Red (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | National flag red (Turkish Flag Law). The field; by tradition the blood of martyrs and Ottoman sovereignty. |
| **Türkiye** | White | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | National flag white. The crescent and star — peace, purity, and the Turkic/Islamic heritage symbol. |

The schemes anchor each editor's foreground on the national **white** and its keywords, cursor, selection, and status line on the exact national **blue** (`#0D5EAF`) or **red** (`#E30A17`), so the colours that matter are literally the national colours. These are interpretive design palettes; the flag-colour reference is provided for provenance and is not an official representation of either state. Colour meanings follow the Hellenic Republic MFA state-symbols description and the Turkish Flag Law tradition.


### Color Swatch Table

The palette cells below use **SVG swatches**, so GitHub renders the actual colors rather than empty HTML color cells. Each strip shows the six colors in this order: background, foreground, keywords, types, strings, and comments. The exact hex codes are listed beside each swatch, in the same scheme order as the reference table above.

| Scheme | Rendered palette colors | Hex codes (background · foreground · keywords · types · strings · comments) |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Additional accent colors are used for functions, constants, selections, diagnostics, and preprocessor directives. The Russian and Slavic palette intentionally uses medium-intensity slate, blue, sage, ruby, and antique gold for a calmer professional appearance.

### Install the Vim Schemes

From the repository root:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Add the following to `~/.vimrc` to enable SLeeLa syntax highlighting and choose a scheme:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Choose exactly one of these colorscheme names:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

For example, use `colorscheme sleela-russian-slavic` for the medium Russian and Slavic palette, or `colorscheme sleela-greek` / `colorscheme sleela-turkish` for the Hellenic and Al Bayrak palettes. Restart Vim or source your configuration with `:source $MYVIMRC`.

### Install the Emacs Schemes

Create a local theme directory and copy the mode and theme files from the repository root:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Add this to `~/.emacs` or `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Replace `sleela-american` with `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro`, or `sleela-sleela` to select another scheme. If Emacs asks whether to trust the theme, inspect the local theme file before accepting it.

### Preview the Native C/C++ Palettes

The small native palette utility can list schemes, print their terminal colors, and look up an individual color:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

For backward compatibility, `--palette` and `--hex COLOR` without `--theme` select American Professional. Build and test results should be checked on the target system and CI; documentation of a command is not itself evidence that it passed.

See [the editor integration guide](linux-editors/README.md) for the full palette descriptions and utility details.

## IX. Contributing
1. Read the documentation index and testing policy.
2. Make focused changes with positive and negative tests.
3. Run targeted tests, then the broad suite.
4. Report OS/toolchain details; do not describe skipped checks as passing.

SLeeLa is developed by **MEARVK LLC**. Historical landing-page material is preserved in [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Round Public Email Levy Expectation

For the AE6E66 initiative, the project's stated expectation is **$300,000,000.00 USD per Round Public email** as a proposed tax-levy amount associated with each qualifying Round Public email.

This statement records a project expectation only. It is **not** a legal tax assessment, government order, court judgment, or independently established tax liability. Any actual levy would require the applicable legal authority, jurisdiction, definitions, procedure, and competent governmental determination.


## XI. Alphabet and Character-set Runtime

SLeeLa's alphabet documentation and native character runtime are maintained in alphabet/sleela/ and lib/alphabet/native/. The native C++17 tests cover strict UTF-8 scalar decoding/encoding, C<n> capacity signatures, and bounded character-set membership:

    make -C lib/alphabet/native test

The experimental UTF-4088 catalogue and its SLeeLa lookup adapter are documented in utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. UTF-4088 IDs are a separate namespace, not Unicode scalar values. Compiler and SLVM binding work is explicitly tracked separately from these native foundations.

## XII. AutoCAD Creator™

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


## XIII. UTF-4088 Input-map Ordering

The specialized UTF-4088 catalogue defines its spatial input-map traversal and abstract pressure iterations in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). The National Lien subject-index policy is in [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Map coordinates, pressure levels (1–50), permanent catalogue IDs, and alphabetic ranks are independent fields; none should be silently substituted for another. Pressure is an algorithmic iteration level, not a physical quantity.


## XIV. Recent and Expanded Package Index

This appended index highlights recently expanded packages, domain libraries, and
source collections. Entries are grouped alphabetically by package name or topic.
Descriptions identify the repository location; implementation and validation
status should be checked in each package's own documentation and tests.

### A–C

- **Advantage** — `lib/advantage/` and `advantage/`: advantaging a unique
  dividend with respect to a model as a layered Sequitur onion, with five
  subject subfolders — `math/` (joint profit maximization, Nash bargaining,
  bilateral newsvendor), `physics/` (trust-field metric/geodesic/contact/SDE,
  a flagged modelling analogy), `integration/` (vertical-integration
  compartments and a sophistication score), `grading/` (an A–D sophistication
  circuit grader for modules and sets), and `output/` (terminal / CommonRails /
  socket emit). A native C/C++ consumer (`advantage/`) links the model into
  worked trade examples. See [`lib/advantage/README.md`](lib/advantage/README.md)
  and [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet and UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/`, and
  `utf-4088/`: alphabet catalogues, native character-set support, input-map
  ordering, and UTF-4088 integration documentation.
- **AutoCAD Creator** — `autocad/creator/` and `lib/autocad/`: interactive
  2D drawing workflows and reusable CAD document/editing components.
- **Ballum** — pixel-level display control: per-pixel set/get, region blit,
  whole-display brightness/warmth/gamma, pointer/keyboard input, and a
  **fullscreen framebuffer surface** for total, owned, fast per-pixel control
  (override-redirect window + single `XPutImage`/MIT-SHM present on Linux/X11).
  Native core under `ballum/` with the SLeeLa facade in `lib/ballum/` and a VM
  bridge under `impl/`; see [`ballum/README.md`](ballum/README.md) and the
  runnable [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` and `lib/cdma/`: CDMA-related technical material and
  SLeeLa source-library work.
- **Character** — `lib/character/`: character profiles and reusable character
  statistics, including biographical profile examples.
- **Coorenagraph** — `lib/coorenagraph/`: ethical reasoning, moral vocabulary,
  deliberation, and related concepts; see its package README and source files.

### D–G

- **Governance source models** — [lib/governance](lib/governance/README.md) defines declarative SLeeLa source shapes for proposals, anonymous manual ballots, and aggregate tallies. The proposed @vote annotation and manual process are documented in [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md); current compilers treat it as inert comment metadata.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` through the available versioned HTTP
  package directories: version-specific protocol specifications and
  implementations. Consult each version's documentation for its precise scope
  and verification status.
- **International Criminal Court** — `lib/international-criminal-court/` and
  `INTERNATIONAL.CRIMINAL.COURTS.md`: related domain-specific source and
  documentation.

### O–S

- **OFDMA** — `ofdma/`: orthogonal frequency-division multiple-access
  technical material and C/C++ development resources.
- **Philosophy and the Conversation Mask module** — [`philosophy/`](philosophy/README.md)
  contains the native C/C++ foundations and [`philosophy/module/`](philosophy/module/README.md)
  organizes their next module layer into three explicit areas:
  [`data/`](philosophy/module/data/README.md) for typed, dated observations,
  event endings, half-life settings, provenance, and consent; [`helper/`](philosophy/module/helper/README.md)
  for deterministic validation, relevance decay, compatible comparisons, and
  consent-filtered presentation; and [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  for a supportive, non-clinical conversation interface. The SLeeLa-facing
  handles and composed Tree live in [`lib/philosophy/`](lib/philosophy/README.md),
  alongside the wider philosophy package. The module is explicit-input and
  consent-first: it does not passively monitor people or media, diagnose, infer
  private intent, score human worth, or recommend medication changes. Its
  event links preserve provenance and navigation, not independent corroboration.
- **Synchro** — native runtime/compiler integration and packet-dispatch and
  measurement-related functionality; see the relevant `impl/` source and
  compiler implementation.
- **SleelaUI / terminal UI** — `lib/user-interface/` and the
  `sleela-terminal` project: SLeeLa UI integration and terminal-oriented
  interaction components.

### U–W

- **United Nations** — `united-nations/`: domain-specific repository materials.
- **Wi-Fi 4, Wi-Fi 5, and Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/`, and related `lib/wifi/` source directories: generation-specific
  wireless networking material and configuration resources.

### Package-index Maintenance

When a package is added or materially expanded, add its entry here in the
appropriate alphabetical group, link to its canonical directory and README,
and describe only capabilities supported by the checked-in sources. Keep this
index additive: do not remove or rewrite the existing README sections as part
of package-index updates.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. National Science Brand Imagery

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. UTF-4088 Million-Entry Experimental Catalogue

The [UTF-4088 catalogue generator](utf-4088/tools/generate_catalogue.py) deterministically builds a 1,048,576-entry experimental symbol catalogue. The documented compact format is described in [utf-4088/FORMAT.md](utf-4088/FORMAT.md); the generation and validation contract is in [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Run the generator with Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

The generated binary is 16,777,244 bytes and is designed to remain below 20,000,000 bytes. Unit tests and a GitHub Actions workflow verify entry count, glyph uniqueness, size, and SHA-256; successful runs publish the binary and summary as the `utf4088-million-catalogue` workflow artifact. The generated binary is deliberately kept out of Git history. UTF-4088 remains experimental and is not Unicode-compatible.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Recent Additions — Pi Constant, Modium/Registry Startup, sleelactl Math, and `@native`

This section records features added during the 2026-11 work, newest groups first. The linked source files and their validation status are authoritative; changes under review are not described as merged until they reach `master`.

### The 100,000,000-digit Pi catalog constant

The repository ships a verified **one-hundred-million-digit** value of π at [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (a well-formed decimal, `3.14159…`), alongside a 57,344-digit value [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Both are independently verified genuine π — see [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), which documents the checks (textbook prefix, the Feynman point at fractional position 762, the famous one-millionth digit, agreement with `lib/math/SLPiReference.sleela`, and a whole-file digit-frequency analysis) and a one-shot reproduction script.

### Modium™ / Registry™ load the Pi constant at subsystem startup

On the dedicated subsystem entry points — `sleela registry` and `sleela modium` (and the installer first run), **not** on every `sleela run` — the Modium and Registry subsystems load the large π catalog constant. On an interactive terminal they print a transient red `Loading Modium™` / `Loading Registry™` line that is then erased in place before normal output begins; the effect is strictly TTY-gated, so piped and test output stay byte-for-byte unchanged. The locator (not the bytes) is seeded as the registry `Math` key defaults `piReferenceLargePath` and `piReferenceLargeDigits`, in both the SLeeLa model ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) and the native seeder ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` exposes `largePath()` / `largeDigits()`. The π constant is installed into the OS SLeeLa location (`<data>/pi/`) by **both** the standard `make` installer and the guided "beautiful" installer. Details: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — the beautiful installer and a native-backed `math` subject

- The guided ("beautiful") installer now installs `sleelactl` (the SLeeLa control plane) and its module registry, matching the `make` installer. See [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` gained a new **`subject`** nature for the native C/C++ subject libraries compiled into the runtime, and registers the first one: **`math`**. `sleelactl run math` exercises the native `__NativeMath` backend (`impl/subjects/math/math_native.cpp`) end-to-end via the subject's reference demo; `sleelactl info math` explains how it is reached (`import math;`). The subject nature offers `run info` only — there is deliberately no `check` verb, because `sleela check` does not load the SHEET-registered native subjects.

### `@native`, Native Drive, and multi-language execution

A new **`@native`** method annotation requests ahead-of-time native compilation of a hot method. The new `sleela aot` command finds `@native` methods, verifies each is within a supported *offloadable subset*, and emits C/C++ — **failing closed** (refusing, with a precise reason) on any construct it cannot yet translate, so a half-translated method is never produced.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` remains a request for AOT treatment, with unsupported constructs rejected before emission. The [Native Drive](impl/native-drive/README.md) supports multi-unit C/C++ executable and shared-library builds through host compilers (including MSVC), supported SLeeLa AOT output, symbol inspection against the native-loader ABI, shell-free process execution, stdin/stdout/stderr handling, multi-stage piping, and XClass delegation to the production loader. `sleelactl` exposes its build/run commands and routes XClass inspection and explicit systemd unit rendering/installation through `inspect native-drive`. The VM owns a bounded native-module handle table with mandatory host authorization and generation-checked handles. Syntax 3.0 now adds `nativeOpen`, `nativeCall`, and `nativeClose`: append-only bytecode dispatch crosses a separately versioned null/bool/int64/double/UTF-8 ABI, while VM-owned arrays, structs, and other composite values stay inside the interpreter and may use same-name bytecode fallback. Plugins can optionally receive a versioned host callback table to invoke exact SLeeLa functions through nested VM execution, with suspended-stack GC rooting, an explicit host callback-name allowlist, and recursive native API calls rejected. Native code remains trusted in-process code, not a sandbox. Source process-policy annotations have a versioned schema but are not yet parsed/enforced. Details, security boundaries, and the seven-workstream ledger: [Native Drive](impl/native-drive/README.md) and [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. International README Editions

SLeeLa provides country-specific README entry points for the 50 markets in its international edition. Each country has one file named `README.<CC>.md`; where multiple languages are relevant, they are grouped within that country's file.

- [README.AE](README.AE.md)
- [README.AT](README.AT.md)
- [README.AU](README.AU.md)
- [README.BE](README.BE.md)
- [README.BR](README.BR.md)
- [README.CA](README.CA.md)
- [README.CH](README.CH.md)
- [README.CL](README.CL.md)
- [README.CN](README.CN.md)
- [README.CO](README.CO.md)
- [README.CZ](README.CZ.md)
- [README.DE](README.DE.md)
- [README.DK](README.DK.md)
- [README.EE](README.EE.md)
- [README.ES](README.ES.md)
- [README.FI](README.FI.md)
- [README.FR](README.FR.md)
- [README.GB](README.GB.md)
- [README.GR](README.GR.md)
- [README.HK](README.HK.md)
- [README.HU](README.HU.md)
- [README.ID](README.ID.md)
- [README.IE](README.IE.md)
- [README.IL](README.IL.md)
- [README.IN](README.IN.md)
- [README.IS](README.IS.md)
- [README.IT](README.IT.md)
- [README.JP](README.JP.md)
- [README.KR](README.KR.md)
- [README.LU](README.LU.md)
- [README.MX](README.MX.md)
- [README.MY](README.MY.md)
- [README.NL](README.NL.md)
- [README.NO](README.NO.md)
- [README.NZ](README.NZ.md)
- [README.PH](README.PH.md)
- [README.PL](README.PL.md)
- [README.PT](README.PT.md)
- [README.QA](README.QA.md)
- [README.RU](README.RU.md)
- [README.SA](README.SA.md)
- [README.SE](README.SE.md)
- [README.SG](README.SG.md)
- [README.SI](README.SI.md)
- [README.SK](README.SK.md)
- [README.TH](README.TH.md)
- [README.TR](README.TR.md)
- [README.TW](README.TW.md)
- [README.US](README.US.md)
- [README.ZA](README.ZA.md)

**Translation policy:** These files are initial localized country entry points. Their introductions and language headings are localized, while the complete technical reference remains in the canonical English [README.md](README.md). A country file must not be described as a complete translation until every section has been translated, technical identifiers and links have been checked, and the edition has received language review. Language coverage in this first pass is representative and is not a claim that every constitutionally recognized, regional, indigenous, or sign language has been fully translated.

## Appendix — retained multilingual guidance (Afrikaans and other South African languages)

The practical guidance below was retained from the previous South Africa edition. Where it differs from the canonical README, the canonical README prevails.

### I. Start here

- [Build the native implementation](impl/README.md)
- [Virtual-machine and Modium user guide](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [CPU architecture in SLeeLa source](lib/cpu/CPU.md) — more than 130 CPU profiles; see [timing controls](lib/cpu/TIMING.md).
- [Documentation index](docs/README.md)
- [Generated API documentation](docs/api/README.md) and [API catalogue](api/README.md)
- [Platform support and validation](docs/PLATFORM-SUPPORT.md)
- [Testing policy](markdown/TESTING.md)

### II. Build and test

Requirements include a C/C++ toolchain (GCC or Clang), GNU Make, and Python 3. Linux graphical components may also require X11/Xrandr development packages.

Run from the repository root:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

A successful build of one component does not establish that every library or platform is release-ready. Consult the build and platform documentation for target-specific requirements.

### III. Install

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

The conventional system prefix is generally `/usr/local`. Setting `PREFIX="$HOME/.local"` provides a user-local installation and normally avoids administrator privileges. See the [installation guide](install/README.md) for details.

### IV. Language and runtime

The normative grammar is [`SLEELA.syntax`](SLEELA.syntax). This repository documents syntax **3.0**. Compiler version and source-language syntax version are distinct.

Technical references cover `@mix` contiguous fields, extended integer families, Preonem document markers, SleelaDoc, API documentation generation, and Nordshrift. Technical identifiers, commands, filenames, API names, and code examples remain unchanged.

### V. Engineering and validation

SLeeLa emphasizes evidence, explicit component contracts, portability, safe failure behavior, and reproducible releases. Validation must be assessed by module and platform. Skipped checks must not be reported as passing; one successful component build is not full release validation.

Sleelavirin protection tooling includes a local SHA-256 verifier, an optional ClamAV adapter, and an integrity manifest. A release should be considered validated only after its commit's CI checks finish and the resulting artifacts are reviewed.

### VI. Editors and contributions

SLeeLa provides **Vim** and **Emacs** themes and syntax integration, plus a native C/C++ palette utility. See the [editor integration guide](linux-editors/README.md).

Contributions should be focused, include positive and negative tests, and state the operating system and toolchain used. Do not describe skipped checks as successful.

SLeeLa is developed by **MEARVK LLC**.

### VII. Other technical areas

The main README also covers the Advantage, SleelaDoc, and XML DOM libraries; primitive integer types; the character runtime; AutoCAD Creator™; the experimental UTF-4088 catalogue; Pi, Modium, Registry, and `sleelactl` tools; and the initial native C/C++ compilation path enabled by the `@native` marker.

### Complete technical reference

The [canonical README](README.md) contains the complete feature catalogue, language and runtime reference, build and installation instructions, API links, examples, and release notes. This South African edition expands the practical English guide and retains the multilingual introductions, but it is not a complete line-by-line translation of the canonical technical README. English remains the normative technical source until reviewed translations are available.

- [Canonical README](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**Translation status:** Practical technical guidance is expanded in English. Full technical translations into the listed South African languages, including South African Sign Language resources, remain outstanding and should be reviewed by fluent technical contributors.


### VIII. Afrikaanse tegniese gids

#### Samestelling en uitvoeringsmodel
SLeeLa skei die brontaal, die samesteller se frontend, die intermediêre voorstelling, kodegenerering en runtime-uitvoering. Die samestellerweergawe, sintaksuitgawe en VM/ISA-weergawe is verskillende weergawe-asse. 'n Funksie wat in die spesifikasie genoem word, is nie noodwendig op elke teiken geïmplementeer en bekragtig nie.

Die pyplyn begin met UTF-8-`.sleela`-bronlêers, voer leksikale en sintaktiese ontleding uit, kontroleer simbole en tipes, verlaag 'n gestruktureerde intermediêre voorstelling, genereer uitset vir ondersteunde VM- of intermediêre teikens en voer die resultaat in die runtime uit. Die normatiewe grammatika is [SLEELA.syntax](SLEELA.syntax); sien ook [source documentation](markdown/SOURCE.md) en [compiler documentation](markdown/COMPILER.md).

#### Tipes en objekmodel
Die gedokumenteerde brontipes sluit `int`, `double`, `boolean`, `String`, `void`, verklaarde `struct`-tipes en ondersteunde reference-type signatures in. `int`, `long` en `int64` is getekende 64-bis heelgetalle. `int128` word as 'n heelgetaltipe aanvaar, maar die huidige gedokumenteerde implementering is steeds 64-bis-gegrond; moenie volle 128-bis-rekenkunde aanvaar nie.

'n `struct` is 'n benoemde aggregate. VM-bestuurde struct-handvatsels is nie rou wysers nie; toekenning en parameteroordrag deel dieselfde instansie, en gelykheid berus op identiteit. Gebruik `structPack(instance)` en `structUnpack(TypeName, json)` vir die gedokumenteerde draagbare serialisering. 'n VM-plaaslike handvatsel is nie self 'n oordraagbare netwerkwaarde nie. Klasse bevat velde en metodes; huidige klasvelde is gedeelde toestand, terwyl metode-plaaslike veranderlikes thread-plaaslik is. Oorerwing, geneste klasse en generics is aan sintaksweergawepoorte onderhewig.

#### Lusse, tydsberekening en biblioteke
Die stellingsvlak-aantekeninge `@loopDelay` en `@loopBudget` bepaal 'n lusvertraging of totale tydbegroting. Hulle word na VM-instruksie `OP_SLEEP` (ISA 157) verlaag; die ingeboude `sleep(ms)` gebruik dieselfde instruksie. Sien [SLEELA.syntax §32](SLEELA.syntax) en [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).

Belangrike gedokumenteerde biblioteke sluit in `lib/advantage` (modelgebaseerde evaluering, wiskunde, integrasie, gradering en roeteerbare uitset), `lib/xml` (XML DOM, ontleding en serialisering), `lib/text` (teks- en stringbewerkings), `lib/sleeladoc` (brondokumentasie en CLI-verwysings), `lib/cpu` en `lib/xlt` (CPU-profiele en model-/tydsberekeningverwysings), en `recursion/stanchion` (C/C++-rekursieprototipes). 'n Module se teenwoordigheid bewys nie produksiegereedheid nie; hersien sy toetse en platformbeperkings.

#### C/C++, bouwerk en installering
Die repository bevat C/C++-komponente en 'n beperkte native-kodegenereringspad. `@native` en `sleela aot` is ontwikkelingsmeganismes, nie algemene toestemming om arbitrêre native kode uit te voer nie. C/C++-grense moet ABI, oproepkonvensie, geheue-eienaarskap, datauitleg, foutafhandeling en platformafhanklikhede uitdruklik definieer. 'n Reference identifiseer 'n hulpbron, maar magtig nie uitvoering nie.

'n Tipiese bou- en toetsreeks:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Stelselinstallering gebruik gewoonlik `sudo make install`; gebruikersinstallering kan `make install PREFIX="$HOME/.local"` gebruik. Bouwerk vereis gewoonlik GCC of Clang en GNU Make; sommige gereedskap gebruik Python 3. Raadpleeg [installation guide](install/README.md) en [platform matrix](docs/PLATFORM-SUPPORT.md).

#### Integriteit, veiligheid en bydraes
Hash-manifeste en verifikasiegereedskap vergelyk lêers met verwagte inhoud. 'n PASS-resultaat geld slegs vir die verklaarde omvang en tydstip. Toetsverslae moet pass, fail en skip onderskei; toetse wat oorgeslaan of nie uitgevoer is nie, mag nie as geslaag gerapporteer word nie.

Lees [testing policy](markdown/TESTING.md), [documentation index](docs/README.md), [generated API reference](docs/api/README.md) en die relevante module-README voordat jy bydra. Voeg positiewe en negatiewe toetse by, teken OS/toolchain/opdragte/werklike resultate aan en werk dokumentasie by wanneer openbare gedrag verander.

### IX. Ander Suid-Afrikaanse tale en toeganklikheid
Hierdie Afrikaanse tegniese afdeling vul die bestaande kort inleidings in isiZulu, isiXhosa, Sepedi, Setswana, Sesotho, siSwati, Tshivenda, Xitsonga en isiNdebele aan. Dit is nie 'n volledige tegniese vertaling in al elf amptelike tale nie. Suid-Afrikaanse Gebaretaal vereis toeganklike video- of gebaretaalmateriaal en kan nie deur 'n geskrewe taalopsomming vervang word nie. Verdere vertalings behoort deur moedertaalsprekers met tegniese kennis nagegaan te word.

**Translation status:** The South African edition now includes a substantial Afrikaans technical guide alongside the English practical guide and multilingual introductions. Full technical translations into all South African languages and dedicated South African Sign Language resources remain outstanding.


### X. Gevorderde taal- en VM-konsepte

#### Teikens en profiele

SLeeLa onderskei die taal se bronvlak van die uitvoerteikens. Die gedokumenteerde teikenfamilie sluit `SLVM`, `SLJVM` en `SLIR` in; `NATIVE` is 'n afgebakende, voorwaardelike teiken en moet nie as 'n universeel beskikbare uitvoermodus aanvaar word nie. Die profiele `BASIC_COMPLETE` en `ADVANCED_TOTAL` beskryf verskillende omvangsvlakke. Bevestig altyd die werklike teiken, profiel en platformondersteuning vir die betrokke bou.

#### Veiligheidsgrens en bevoegdheid

'n Verwysing identifiseer 'n bron, lêer, module of hulpbron; dit verleen nie vanself toestemming om dit uit te voer nie. Hierdie onderskeid is belangrik wanneer 'n program lêers, eksterne biblioteke, native bindings of ander stelselhulpbronne gebruik. Valideer invoer, beperk die uitvoergrens en dokumenteer watter komponent die bevoegdheid besit. Moenie 'n dokumentasieverklaring as bewys van 'n runtime-sekuriteitswaarborg beskou sonder 'n toepaslike toets of implementeringsverwysing nie.

#### Geheue, toestand en gelyktydigheid

Dokumenteer die eienaarskap en lewensduur van elke hulpbron wat tussen funksies of drade gedeel word. Onderskei plaaslike veranderlikes, gedeelde objektoestand, VM-handvatsels en serialiseerbare data. Moenie VM-lokale handvatsels as stabiele lêerformate of netwerkboodskappe gebruik nie. Gebruik die module se gedefinieerde serialiseringskoppelvlak en toets ongeldige, ontbrekende en grenswaardes.

### XI. Praktiese foutopsporing en vrystelling

Begin met 'n skoon werkboom en teken die commit, bedryfstelsel, argitektuur, compiler en weergawes aan. Bou die kleinste betrokke teiken voordat jy die volledige toetssuite uitvoer. Wanneer 'n fout voorkom, bewaar die volledige uitvoer, onderskei compilerfoute van runtime-foute en verminder die voorbeeld tot 'n klein reproduseerbare program. Moenie 'n suksesvolle bou as bewys van korrekte runtime-gedrag rapporteer nie.

'n Nuttige verifikasieverslag noem:

1. Die presiese commit en plaaslike veranderinge.
2. Die bedryfstelsel, argitektuur en compiler/toolchain.
3. Die presiese bou- en toetsopdragte.
4. Die aantal geslaagde, mislukte en oorgeslane toetse.
5. Bekende beperkings en nie-getoetste teikens.

Die internasionale README-lêers is landgerigte toegangspunte; hulle vervang nie die normatiewe taalspesifikasie of module-API-dokumentasie nie. Hou vertaalde beskrywings in lyn met die Engelse bron en behou identifiseerders, opdragte, lêerpaaie en kode onveranderd.

**Uitgebreide vertalingstatus:** Die Afrikaanse afdeling dek nou ook teikenprofiele, VM-veiligheidsgrense, hulpbron-eienaarskap, foutopsporing en vrystellingsverifikasie. Dit bly 'n substansiële tegniese gids, nie 'n volledige vertaling van elke Engelse afdeling nie.


### XII. IsiZulu: umhlahlandlela wobuchwepheshe

#### Izinhloso zokwakha nokusebenza

I-SLeeLa ihlukanisa ikhodi yomthombo nezinhloso zokwakha nokusebenza. Izinhloso ezibhalwe kumadokhumenti zifaka i-`SLVM`, `SLJVM` ne-`SLIR`; i-`NATIVE` inemikhawulo nemibandela, ngakho akufanele kucatshangwe ukuthi iyatholakala kuwo wonke amapulatifomu. Amaphrofayela `BASIC_COMPLETE` kanye `ADVANCED_TOTAL` amelela ububanzi obuhlukile. Qinisekisa inhloso, iphrofayela nokusekelwa kweplatifomu ngaphambi kokwakha.

I-reference ikhomba ifayela, imojuli noma insiza; ayinikezi ngokuzenzakalelayo imvume yokuyisebenzisa. Uma uhlelo lusebenzisa amafayela, imitapo yolwazi yangaphandle, ama-native bindings noma izinsiza zesistimu, chaza ngokucacile imingcele yemvume nokuthi iyiphi ingxenye enegunya lokwenza isenzo. Incazelo esencwadini ayibona ubufakazi obanele besiqinisekiso sokuphepha se-runtime; hlola ikhodi nokuhlolwa okuhambisanayo.

#### Izinhlobo zedatha nezinsiza

Hlukanisa okuguquguqukayo kwendawo, isimo sezinto esabiwe, ama-VM handles nedatha engaguqulelwa ku-serialization. I-VM-local handle ayiyona ifomethi yefayela ezinzile noma umlayezo wenethiwekhi. Sebenzisa i-serialization interface echazwe yimojuli futhi uhlole amanani angalungile, angekho kanye nalawo asemingceleni.

#### Ukwakha, ukuhlola nokulungisa amaphutha

Imiyalo ejwayelekile:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Ukufaka ohlelweni kuvame ukusebenzisa `sudo make install`; ukufaka endaweni yomsebenzisi kungasebenzisa `make install PREFIX="$HOME/.local"`. Bhala phansi i-commit, i-OS, i-architecture, i-toolchain nemiyalo eqondile. Gcina wonke umlayezo wephutha, hlukanisa amaphutha e-compiler namaphutha e-runtime, bese udala isibonelo esincane esingaphindwa. Ukwakha okuphumelele akufakazeli ukuthi uhlelo lusebenza kahle.

Umbiko wokuqinisekisa kufanele uhlukanise `pass`, `fail` kanye `skip`. Ukuhlolwa okungazange kwenziwe noma okweqiwe akufanele kubalwe njengokuphumelele.

### XIII. isiXhosa: isikhokelo sobugcisa

#### Iithagethi kunye neeprofayili

I-SLeeLa yahlula ikhowudi yomthombo kwiithagethi zokwakha nezokusebenza. Iithagethi ezichaziweyo ziquka `SLVM`, `SLJVM` kunye `SLIR`; i-`NATIVE` inemiqathango nemida, ngoko ayifanele ithathwe njengento efumaneka kuzo zonke iiplatform. Iiprofayili `BASIC_COMPLETE` ne `ADVANCED_TOTAL` zibonisa imida eyahlukileyo. Qinisekisa ithagethi, iprofayili nenkxaso yeplatform ngaphambi kokwakha.

I-reference ichonga ifayile, imodyuli okanye isixhobo; ayiniki mvume yokuyiqhuba. Xa inkqubo isebenzisa iifayile, amathala eencwadi angaphandle, ii-native bindings okanye izixhobo zesistim, chaza ngokucacileyo imida yemvume noxanduva lokuqhuba. Inkcazelo ekuxwebhu ayibobungqina bodwa besiqinisekiso sokhuseleko se-runtime; jonga ukuphunyezwa novavanyo olufanelekileyo.

#### Uvavanyo nokulungisa iimpazamo

Rekhoda i-commit, i-OS, i-architecture ne-toolchain. Qala ngethagethi encinane efanelekileyo, gcina yonke imveliso yeempazamo, wahlule iimpazamo ze-compiler kweze-runtime, uze wenze umzekelo omncinane onokuphindwa. Ukwakhiwa okuphumeleleyo akungqini ukuba ukusebenza kwe-runtime kuchanekile.

Ingxelo yokuqinisekisa mayibonise i-commit echanekileyo, i-OS/architecture, iinguqulelo ze-compiler, imiyalelo epheleleyo yokwakha nokuvavanya, kunye namanani e-`pass`, `fail` ne-`skip`. Uvavanyo olutsityiweyo okanye olungazange luqhutywe alunakubalwa njengoluphumeleleyo.

**Inqaku lokugubungela iilwimi:** La macandelo e-isiZulu nesiXhosa aqala ukunika umxholo wobugcisa ongaphezulu kweentshayelelo ezimfutshane. Afuna uphononongo lwabantu abathetha ezi lwimi njengolwimi lweenkobe nabanolwazi lobugcisa; akufanele kubhengezwe njengoguqulelo olupheleleyo lwezikhokelo zonke.


### XIV. API-contracte, versoenbaarheid en toetsstrategie

Wanneer 'n nuwe API bygevoeg word, dokumenteer die insette, uitsette, fouttoestande, data-eienaarskap en moontlike side effects. Voorbeelde behoort normale gebruik én die hantering van invalid of missing input te wys. Moenie gedrag as gewaarborg beskryf tensy die implementation en toepaslike tests dit bevestig nie.

Verdeel toetse volgens die compiler pipeline: lexing/parsing, semantic analysis, IR lowering, code generation, VM execution en native integration. Bewaar vir elke regression case die kleinste reproduceerbare inset, die verwagte resultaat en die werklike resultaat. API-veranderinge moet backward compatibility, migration steps en gevolge vir bestaande examples beskryf.

### XV. Lokalisering en meertalige instandhouding

Hou technical identifiers, commands, paths, syntax en code blocks onveranderd oor taalweergawes heen. Vertaal verduidelikende teks, nie tokens of name wat in source code gebruik word nie. Onderskei Afrikaans, isiZulu en isiXhosa as afsonderlike taalweergawes; moenie aanvaar dat een vertaling die ander gemeenskappe verteenwoordig nie.

Kontroleer Markdown links, opskrifvlakke, code fences en voorbeelde ná elke verandering. Wanneer canonical English documents verander, vergelyk die ooreenstemmende afdelings en werk die vertaling inhoudelik by. Inhoud wat nog nie deur moedertaalsprekers of tegniese beoordelaars nagegaan is nie, moet so gemerk word.

**Translation status:** Sections I–XIX above are the complete English edition. The introductions in isiZulu, isiXhosa, Afrikaans, Sepedi, Setswana, Sesotho, siSwati, Tshivenda, Xitsonga and isiNdebele, and the Afrikaans technical guide in the appendix, are retained from the previous edition and have not yet had a full language review; complete reviewed translations of the technical reference into these languages are pending. The canonical [README.md](README.md) remains the normative technical source.
