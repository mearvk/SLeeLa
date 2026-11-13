<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Canada (CA)
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>A native systems language, compiler, runtime, and virtual-machine ecosystem.</strong></p>
> Complete English technical edition for readers in Canada. The full project documentation, code examples, build and installation instructions, API references, and release notes are reproduced. Technical identifiers and repository-relative links are unchanged.
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

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


---

# SLeeLa 3.0 — Canada (CA)

## Français

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Un langage système natif, son compilateur, son environnement d’exécution et son écosystème de machines virtuelles.</strong></p>

> **Édition canadienne en français.** Traduction technique complète destinée aux lecteurs francophones du Canada. Les identifiants techniques, le code, les commandes et les liens restent inchangés.

**The Now. The Ingredient.** Un langage ne se réduit pas à sa syntaxe : il comprend le compilateur, l’environnement d’exécution, les bibliothèques, les outils et les contrats de plateforme qui rendent les logiciels utilisables.

**Les domaines d’activité de SLeeLa.** SLeeLa s’intéresse à **Math**, **Object**, **Science** et **Reach** — les quatre dimensions de son travail : ce qui est calculable (Math), la chose établie dans le catalogue de ce qui existe (Object), l’étude rigoureuse du vrai (Science) et l’extension de la capacité d’action d’un système vers d’autres systèmes et vers les personnes (Reach). **Les intérêts nationaux demeurent** : aucune de ces quatre dimensions ne les écarte. Lorsqu’un sujet concerne l’intérêt d’une nation, cette dimension est conservée et fidèlement consignée, notamment dans les métadonnées de chaque classe, telles que `metaOrigin` et `metaNationalSchedule`.

## I. Pour commencer

- [Construire l’implémentation native](impl/README.md)
- [Machines virtuelles et Modium — guide pratique](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Pile CPU dans les sources SLeeLa](lib/cpu/CPU.md) — une machine complète allant des portes logiques à l’ALU, aux cœurs, au système d’exploitation et aux programmes ; plus de 130 profils CPU et des [tensions, fréquences variables et temporisations avancées](lib/cpu/TIMING.md) réglables, permettant une exécution [aux spécifications ou en surfréquence](lib/cpu/components/README.md).
- [Index de la documentation](docs/README.md)
- [Source de données CSPML et référence de recherche Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — provenance et conseils d’intégration de données externes américaines sur les lieux, les conditions socioéconomiques, la main-d’œuvre, l’éducation et les écoles publiques.
- [Documentation API générée](docs/api/README.md) — référence HTML actuelle, édition versionnée de l’API et instructions de régénération.
- [Catalogue des API](api/README.md)
- [Normes de comparaison computationnelle](comparison/README.md) — normes de types, mémoire, ABI et conformité pour la VM SLeeLa, Java/JVM et le C/C++ natif.
- [Stanchion de récursion — prototypes C et C++](recursion/stanchion/README.md) — exemples spécialisés pour les types primitifs Boolean, Byte, Character, Integer, Long, Float et Double, ainsi que des modèles de récursion génériques, appliqués, scientifiques et pédagogiques.
- [Matrice de prise en charge et de validation des plateformes](docs/PLATFORM-SUPPORT.md)
- [Guide de marque](docs/BRAND-GUIDE.md)
- [Concepts et ressources de produits dérivés](merchandise/README.md)
- [Logo NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Politique de test et critères de publication](markdown/TESTING.md)

## II. Actualités

Ajouts récents au langage, aux pilotes et à la bibliothèque standard :

- **Paquet de bibliothèque standard `advantage` — optimisation d’un dividende, avec mathématiques, physique, intégration, évaluation et sortie routable.** [`lib/advantage/`](lib/advantage/README.md) encapsule l’« optimisation d’un dividende unique par rapport à un modèle » sous la forme d’un **Sequitur onion** à couches (dividende → révision → préoccupations → mathématiques → additions → soustraction → regroupement → itération), navigable vers l’intérieur et l’extérieur avec mise à jour dynamique et restitution complètes, au moyen d’un **modèle utilisateur** simple et d’un **modèle développeur** offrant un contrôle intégral. Il comprend cinq domaines : **`math/`**, économie du commerce bilatéral (maximisation conjointe des profits, négociation de Nash, ratio critique bilatéral du modèle newsvendor) ; **`physics/`**, « physique des champs de confiance » appliquée à l’économie structurelle (confiance comme tenseur métrique, géodésique transactionnelle, structure de contact d’annonce et SDE de décroissance de confiance CIR avec sauts — une analogie de modélisation explicitement signalée) ; **`integration/`**, modèle d’intégration verticale composable récursivement pour le capitalisme et le commerce de détail (compartiments, baies d’ingénierie, compartiments d’essai, plateaux de médiane newtonienne et score de sophistication du système) ; **`grading/`**, évaluateur de circuits de sophistication attribuant des notes A–D à chaque module et à tout ensemble de modules gagnants, perdants, évaluateurs ou absolus ; et **`output/`**, interface d’émission routable vers le terminal, une surface au format **CommonRails** ou un socket/connecteur (prévue pour un `slnet_write` natif). Chaque classe porte l’enregistrement Metadata-Class ; cinq démonstrations exécutables valident les domaines. Les documents complémentaires [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md), [`MATH.md`](lib/advantage/MATH.md) et [`PHYSICS.md`](lib/advantage/PHYSICS.md) les décrivent. Un consommateur natif C/C++ dans [`advantage/`](advantage/README.md) — noyau C `advantage_value`/`advantage_mean`, magasin de connaissances XML et bibliothèque sécurisée d’acquisition HTTPS/SFTP/SSH, magasin de clés et transport DH/ECDH — relie le modèle à des [exemples de commerce](advantage/examples/README.md) : transactions positives issues de sources Internet supposées, séquence de pertinence pour Traders, Consumers, Helpers et Healers, et huit modèles classés de A à D (D = Cement maximal ; A = Brand maximal).
- **Annotations de temporisation des boucles — `@loopDelay` / `@loopBudget` et fonction intégrée `sleep(ms)`.** Il s’agit des premières annotations **comportementales au niveau de l’instruction** de SLeeLa : écrites sur leur propre ligne avant une boucle `for`, `while` ou `do`, elles insèrent une véritable pause dans son exécution. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` attend une durée fixe par itération (la forme abrégée `@loopDelay(250)` utilise les millisecondes par défaut) ; `@loopBudget(seconds = 10, iterations = 1000)` répartit un nombre connu d’itérations sur un budget total de temps réel. Les deux annotations sont abaissées vers le nouvel opcode append-only **`OP_SLEEP`** (ISA 157), qui prend également en charge la fonction intégrée générale **`sleep(<ms>)`**, appelable partout. La pause est implémentée de façon portable par `nanosleep` / `Sleep`. Spécification : [`SLEELA.syntax` §32](SLEELA.syntax) ; référence API : [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md) ; démonstration exécutable : [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela). Un index des ajouts de session — comprenant également la multiplication native de grands entiers `OP_BIGMUL` et les outils autonomes [local-generator](local-generator/README.md) et [pi-generator](pi-generator/README.md) — se trouve dans [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md). Les trois surfaces API générées ([`sleeladoc-api/`](sleeladoc-api/README.md), [`docs/api/`](docs/api/README.md) et l’instantané immuable [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)) ont été reconstruites avec `make api-docs` et incluent désormais les entrées `SLISASleep` / `SLISABigMul` sur la page du paquet `vm`.
- **SleelaDoc depuis la ligne de commande — `sleela docs`.** SleelaDoc est désormais accessible directement depuis le terminal. `sleela docs` répertorie tous les sujets documentables ; `sleela docs crypto` affiche un paquet ; `sleela docs stringbuilder` résout une classe sans distinction de casse et reconnaît le préfixe standard `SL` comme `SLStringBuilder`. La commande affiche une page détaillée — **définition, objectif de conception, relations, structure, user clays et échelle de maîtrise à cinq niveaux** — avec des suggestions classées de type « vouliez-vous dire ? » en cas de nom proche. Les auteurs enrichissent chaque page en ajoutant des balises SleelaDoc `@` telles que `@intent`, `@thesis`, `@axiom`, `@param`, `@see` et `@mastery`. `sleela docs --generate [dir]` génère un corpus en texte brut (une page par classe et un index `TOPICS.index`) que la CLI peut ensuite lire hors ligne ; **`make sleeladoc`** regroupe ce corpus et les sources `.sleela` correspondantes dans `sleeladoc/build/sleeladoc.jar`. Pour les sujets ne disposant que d’une façade, comme `psychiatry` et `politico`, `sleela docs --facade <topic>` explore le système de fichiers à l’exécution et énumère les dossiers réels, puis les fichiers, avec des limites configurables par l’utilisateur de **6 Mo, 6 000 fichiers et 6 minutes**. Les unités de `lib/sleeladoc` utilisent l’édition actuelle **`#sleela 3.0`**. Voir [`man/man1/sleela.1`](man/man1/sleela.1) et [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — documentation de style Javadoc et site HTML généré.** **SleelaDoc** est la convention de commentaires documentaires intégrés au code source de SLeeLa (commentaires entre chevrons délimités par `@doc` / `@page` ; voir [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)). [`lib/sleeladoc`](lib/sleeladoc/README.md) est l’**extracteur** qui transforme une unité `.sleela` en page API Markdown ou HTML, à la manière de `javadoc` pour Java, en documentant l’*intention*. Un site HTML complet et navigable pour l’ensemble de `/lib` est généré dans [`/sleeladoc-api`](sleeladoc-api/README.md) par `tools/generate-sleeladoc-api.py` (ouvrir `sleeladoc-api/index.html`) ; il utilise un style bleu, bleu clair, gris clair et blanc, une hiérarchie paquet → classe → membre et un glossaire destiné au lecteur *Student 2*. Le compilateur produit également, en complément de la compilation, des documents marqués par l’auteur (`--wiki` : page MediaWiki et copie jumelle de la source d’origine), décrits dans [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Démonstration exécutable : [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modèle objet de document XML (`lib/xml`).** DOM XML côté SLeeLa : un document possède un arbre de nœuds élément/texte/commentaire/CDATA/instruction de traitement, avec enfants ordonnés et attributs nommés ; il comprend un sérialiseur compact ou lisible (échappement XML 1.0 §2.4), un analyseur par cellules de caractères et le registre qualité `SLXmlAspect`. L’égalité des nœuds suit trois niveaux de confiance (identité → même sujet → structure) et est réflexive, symétrique et déterministe. Voir [`lib/xml/README.md`](lib/xml/README.md) et l’exemple exécutable [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Primitives d’entiers signés de grande largeur (`long`, `int64`, `int128`).** `int`, `long` et `int64` sont tous des entiers signés de 64 bits qui sont vérifiés par le système de types et exécutés de manière identique. `int128` est accepté comme type entier (adossé à 64 bits pour le moment ; un véritable support 128 bits est documenté comme travail ultérieur). Les séries signées en base 2 `int32` à `int16777216` (syntaxe 2.7) et non signées `uint32` à `uint16777216` (syntaxe 2.8) étendent ces capacités. Voir [`SLEELA.syntax`](SLEELA.syntax) §21, §24 et §25.
- **Champs conséquents `@mix` (syntaxe 2.9).** Une classe peut déclarer plusieurs champs d’instance portant le **même nom** lorsque chaque répétition comporte `@mix`. Le langage traite le groupe comme une séquence (tableau / liste indexée), de haut en bas `1,2,3,…` par défaut ou définie manuellement avec `@mix(1,2,3,...)` ; le nombre d’indices doit correspondre au nombre de champs. Voir [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Marqueurs documentaires — le Processus naturel, appelé Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Ces mots nus servant de marqueurs documentaires sont collectivement appelés **Preonems** (un marqueur est un *Preonem* ; l’ensemble, les *Preonems* ; voir [`SLEELA.syntax`](SLEELA.syntax) §26.1). Ce sont des marqueurs documentaires déclaratifs de première classe qui expriment ce qu’un document *est* dans l’ordre de sa création : `author`/`authorial` (syntaxe 2.4 ; géré par le conseil professionnel, seul membre ayant un effet comportemental), `material;` (Source établie) et `memory;` (syntaxe 2.5), `determine;` (syntaxe 2.6 ; document ouvert à un traitement ultérieur par un médecin, praticien ou ghost jusqu’à son achèvement par voie légale), puis les deux plus récents : **`object;`** (chose établie dans le catalogue de ce qui existe, réifiée et dotée d’une existence concrète en syntaxe 2.8) et **`field;`** (champ relationnel dans lequel les objets existent et se déplacent), tous deux disponibles à partir de la syntaxe 2.6. Chaque marqueur est contextuel et conditionné par la version dans le frontend ; il ne modifie ni l’analyse syntaxique, ni le typage, ni la génération de code. En dehors de la position de marqueur, chaque mot reste un identifiant ordinaire : `field` n’interfère donc pas avec un membre de classe ou un accès `.field`. Voir [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 et §26.
- **Nordshrift en syntaxe 3.0 avec accès complet à la bibliothèque.** Nordshrift résout les noms via le même index récursif `/lib` que le compilateur et atteint ainsi tous les paquets et unités `.sleela` ; `nordshrift check` indique les nombres pour l’ensemble de l’arborescence.
- **Stanchion de récursion.** [`recursion/stanchion/`](recursion/stanchion/README.md) comprend désormais des prototypes de référence C et C++ dédiés aux sept contrats primitifs documentés, ainsi que des exemples génériques, appliqués, scientifiques et pédagogiques. Il s’agit de sources de référence : compilez-les et testez-les avant de les considérer comme vérifiées.
- **Commentaires entre chevrons (`<` / `<<`).** Nouveau style de commentaire pour les développements futurs : `< line >` et `<< block >>`, reconnu par le frontend SLeeLa et documenté dans [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Littéraux entiers avec base.** Les littéraux `int` acceptent désormais les formes hexadécimale (`0x`), binaire (`0b`) et octale (`0o`), avec séparateurs de chiffres `_`, dans le langage source SLeeLa ([`SLEELA.syntax`](SLEELA.syntax) §1.4) et dans le format de modèles CPU [`.xlt`](lib/xlt/README.md).
- **Norme Metadata Class dans toute la bibliothèque.** Le métapatron de métadonnées descriptives (provenance, légalité, portée juridictionnelle) est désormais présent dans chaque fichier `.sleela` contenant des classes sous `/lib`. Voir [`SLEELA.syntax`](SLEELA.syntax) §15 et [la leçon du tutoriel](tutorial/13-metadata-class.md).
- **Famille de texte / String enrichie.** `lib/text` ajoute de véritables assistants dans le langage, une vue par cellules de caractères proposant les opérations réelles `charAt`, `substring`, `indexOf`, `reverse` et l’alignement de colonnes, ainsi qu’un moteur de modèles positionnels. Voir le [tutoriel texte et String](tutorial/14-text-and-strings.md) et la démonstration exécutable [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` de première classe dans les feuilles Nordshrift.** Une feuille `.sst` peut déclarer un bloc typé `strings:` qui contribue au manifeste des composants de la compilation. Voir la [référence Nordshrift Sheet](wiki/Nordshrift-Sheet-Reference.md).
- **Modèles CPU exécutés par opcodes et équivalence d’appels de fonction.** Chaque `SL<ARCH>CPU` hérite de la capacité d’exécuter un flux d’instructions natives via sa boucle fetch-decode-execute, ainsi que le code d’un autre processus supposé par des appels équivalents sur l’infrastructure VM partagée. Voir [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Construction et tests

**Prérequis :** une chaîne d’outils C/C++ (GCC ou Clang) et GNU Make ; Python 3 pour les outils de manifeste et de flux ; sous Linux, les paquets de développement X11/Xrandr pour les produits d’affichage. La commande `make deps` d’un module indique ou installe les dépendances nécessaires. La construction de `impl/` échoue de manière fermée si le manifeste SHA-256 de confiance n’est pas respecté ; le `Makefile` racine le référence automatiquement.

Pour démarrer rapidement depuis un clone propre :

```sh
make                                  # construire tous les éléments depuis la racine du dépôt
```

Ou construire et tester uniquement le cœur natif :

```sh
make -C impl all                      # construire uniquement le compilateur et le cœur d’exécution
bash ./test-suites/run-all.sh --all   # exécuter toute la suite de tests
```

La commande `make` à la racine du dépôt construit tous les produits (cœur, VM, codecs, churn, antivirus, etc.), puis termine par la construction du **registre** et la matérialisation de sa table de référence. Un arbre construit contient ainsi un registre vérifiable dès le départ.

Cibles essentielles :

```sh
make                     # tout construire (alias de make all ; registre exécuté en dernier)
make all                 # même construction complète, nommée explicitement
sudo make install        # installer les commandes et les fichiers d’exécution (voir § IV)
make clean               # supprimer les sorties de construction de tous les systèmes invoqués
make help                # afficher toutes les cibles du Makefile racine
```

Autres cibles utiles :

```sh
make registry            # construire uniquement le registre et matérialiser la table
./build-registry.sh      # même opération, exécutée directement
make tests               # construire et exécuter la suite de tests
make ballum              # construire la bibliothèque d’affichage Ballum pour ce système
bin/install-modules      # installation/construction cadencée des modules sur environ 10 à 20 minutes
```

La cible `registry` exécute [`build-registry.sh`](build-registry.sh) : elle construit le cœur du registre, initialise la table de référence dans le stockage texte portable `registry/build/sleela.sleelareg` et exporte le script de table MySQL `registry/build/registry.sql` (avec `sleela registry export`). Le registre peut être examiné avec `sleela registry` ou `registry/build/sleela-registry review`. Voir le [guide du registre](registry/README.md) et l’[architecture Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Consultez les guides de construction et de test pour les prérequis et les étapes propres à chaque plateforme. La réussite de la construction d’un composant ne prouve pas que chaque bibliothèque ou plateforme est prête pour une publication.

## IV. Installation

Après la construction des produits natifs, installez les commandes et les fichiers de prise en charge de l’environnement d’exécution :

```sh
sudo make install
make install PREFIX="$HOME/.local"        # installation pour l’utilisateur, sans sudo
make install DESTDIR="$PWD/package-root" # installation dans un répertoire de préparation de paquet
make install-check
```

L’installateur conventionnel utilise par défaut `/usr/local/bin`, `/usr/local/lib/sleela` et `/usr/local/share/sleela`. Il préserve une configuration de démarrage existante et refuse d’écraser des commandes existantes non suivies par le dépôt. La désinstallation ne supprime que les fichiers enregistrés par l’installateur ; elle préserve la configuration et les données d’exécution non suivies.

Pour un déploiement ciblé et guidé du seul paquet principal — les binaires `sleela`, `sleelvac` et `nordshrift`, la bibliothèque standard, les protocoles et les modules de prise en charge — utilisez l’installateur propre :

```sh
make clean-install       # installation guidée du seul paquet principal
```

Le processus demande d’abord si le paquet principal doit être installé, puis permet de choisir le périmètre. En cas de réussite, un sélecteur facultatif propose des modules d’amélioration sélectionnés. Le déroulement interactif complet et toutes les options sont décrits dans [Installation rapide et sûre](install/README.md).

## V. Langage et environnement d’exécution

La grammaire faisant autorité est [`SLEELA.syntax`](SLEELA.syntax). Le dépôt documente actuellement la syntaxe **3.0** ; la version du compilateur/des outils et la version de syntaxe du code source sont distinctes. La surface 3.0 ajoute les marqueurs documentaires **`pure` et `gasoline`** (§30) : deux Preonems contextuels, déclaratifs et sans effet d’exécution propre, qui expriment chacun la position d’un document par sa propre honnêteté. La syntaxe 3.0 rassemble les surfaces linguistiques 1.0 à 2.9 dans l’édition normative actuelle, tout en conservant les versions minimales requises pour chaque fonctionnalité.

La surface précédente, 2.9, ajoute les **champs conséquents `@mix`** (§28) : une classe peut déclarer plusieurs champs d’instance portant le même nom lorsque chacun est marqué `@mix`. Le langage les traite comme un groupe indexé (de haut en bas `1,2,3,…` par défaut, ou défini manuellement avec `@mix(1,2,3,...)`, dont le nombre d’indices doit correspondre au nombre de champs). Elle reprend également les familles d’entiers larges signés et non signés en base 2, de `int32` à `int16777216` (§24) et de `uint32` à `uint16777216` (§25), qui prolongent `long`, `int64` et `int128` (§21) ; les documents de prise en charge des mots-clés (§22) ; ainsi que les **Preonems** : marqueurs documentaires déclaratifs `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) et `artifact` (§35 ; statut de qualité législative évalué à l’échelle du temps et de l’espace). L’ensemble porte le nom de Preonems (§26.1).

La résolution de surcharge des constructeurs ainsi que la délégation vers `this(...)` et les constructeurs de classe de base sont explicitement hors du contrat de syntaxe 1.8 vérifié. Consultez la grammaire et les notes d’implémentation avant de vous appuyer sur une fonctionnalité. Les neuf unités sources SleelaDoc déclarent désormais `#sleela 3.0` ; le contrôle commun du compilateur accepte les syntaxes source 1.0 à 3.0. La nouvelle construction de l’API est produite par `make api-docs`.

Le dépôt comprend une implémentation native C/C++, des références de génération SLVM, des modules de bibliothèque standard, des tests, des exemples et des intégrations de plateforme. L’état d’implémentation et de validation varie selon les modules ; consultez leurs spécifications et les résultats CI actuels plutôt que de supposer une complétude uniforme.

## VI. Principes d’ingénierie

- **Les preuves avant les affirmations :** distinguer les comportements implémentés, testés, expérimentaux et prévus.
- **Un contrat par frontière :** documenter la syntaxe, les API, les formats de fichiers, les hypothèses de plateforme et les comportements en cas d’échec.
- **Conception portable :** isoler les backends propres aux plateformes et tester sur des exécuteurs natifs.
- **Échec sûr :** tester les entrées mal formées, les limites de ressources, les défaillances de processus et les chemins de nettoyage.
- **Publications reproductibles :** reconstruire proprement et publier les empreintes des artefacts ainsi que les notes de version.

## VII. Construction de l’antivirus et intégrité

Le paquet antivirus Sleelavirin comprend un scanner SHA-256 natif, un adaptateur ClamAV facultatif, des manifestes d’intégrité pour les sources et les index, ainsi que des sommes de contrôle pour les paquets de plateforme. Construisez et testez le paquet depuis la racine du dépôt :

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Le manifeste d’intégrité est généré sous `antivirus/build/`. Il couvre les sources et les documents suivis par le dépôt ainsi que les fichiers d’index de signatures protégés ; les sorties générées et le cache ClamAV modifiable en sont exclus. Les instantanés de bases de données ClamAV disposent de leurs propres manifestes. Les archives ZIP de publication comprennent `INTEGRITY-MANIFEST.json`, `SHA256SUMS` et un fichier compagnon `.sha256`. Vérifiez les empreintes des archives téléchargées à l’aide d’une valeur de confiance obtenue indépendamment : une somme de contrôle stockée à côté d’une archive ne constitue pas une signature de l’éditeur. Voir le [guide Sleelavirin](antivirus/README.md), la [liste de contrôle de sécurité](antivirus/SECURITY.md) et le [workflow de construction antivirus](.github/workflows/antivirus-binaries.yml).

Une publication n’est considérée comme vérifiée que lorsque les tâches CI pertinentes réussissent sur le commit exact de cette publication et que les artefacts obtenus ont été contrôlés.

## VIII. Thèmes de l’éditeur SLeeLa — Vim et Emacs

SLeeLa comprend un ensemble coordonné de thèmes sombres professionnels pour **Vim** et **Emacs**, ainsi qu’un utilitaire natif de palettes C/C++. Le thème d’origine **American Professional** reste le thème par défaut. Dix palettes complémentaires proposent des inspirations de conception culturelles et institutionnelles variées ; il s’agit d’interprétations graphiques et non de représentations officielles d’une nation, d’une culture, d’une cour ou d’une institution.

### Les onze thèmes et leurs couleurs

| Thème | Arrière-plan | Premier plan | Mots-clés | Types | Chaînes | Commentaires |
|---|---|---|---|---|---|---|
| **American Professional** | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| **Pan-Eurasian** | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| **Russian and Slavic — Medium** | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| **Central Asian / Silk Road** | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| **East Asian** | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| **ICC — Cour pénale internationale** | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| **SCOTUS — Cour suprême des États-Unis** | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| **Greek (Hellenic)** | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| **Turkish (Al Bayrak)** | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| **Kiro** | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| **SLeeLa** | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

### Couleurs des drapeaux nationaux (palette source)

Les thèmes grec et turc sont dérivés des **couleurs officielles des drapeaux nationaux** des deux pays, indiquées ici avec leurs valeurs hexadécimales publiées. Les arrière-plans et accents des thèmes ci-dessus sont des interprétations assombries, adaptées aux éditeurs ; le tableau ci-dessous conserve les couleurs de référence telles qu’elles sont exposées au niveau national.

| Pays | Couleur | Hexadécimal | Fondement et signification |
|---|---|---|---|
| **Grèce** | Bleu | `#0D5EAF` | Bleu du drapeau national (loi 851/1978). Le ciel et les mers ; en héraldique, l’azur. |
| **Grèce** | Blanc | `#FFFFFF` | Blanc du drapeau national. Pureté de la lutte pour l’indépendance ; la croix représente le christianisme orthodoxe oriental. |
| **Türkiye** | Rouge (*al bayrak*) | `#E30A17` | Rouge du drapeau national (loi turque sur le drapeau). Le champ ; selon la tradition, le sang des martyrs et la souveraineté ottomane. |
| **Türkiye** | Blanc | `#FFFFFF` | Blanc du drapeau national. Le croissant et l’étoile : paix, pureté et symbole de l’héritage turcique/islamique. |

Les thèmes ancrent le premier plan de chaque éditeur sur le **blanc** national et leurs mots-clés, curseur, sélection et barre d’état sur le **bleu** national exact (`#0D5EAF`) ou le **rouge** (`#E30A17`). Les couleurs importantes sont donc littéralement les couleurs nationales. Il s’agit de palettes interprétatives ; la référence aux couleurs des drapeaux documente leur provenance et ne constitue pas une représentation officielle de l’un ou l’autre État. Les significations des couleurs suivent la description des symboles d’État du ministère grec des Affaires étrangères et la tradition de la loi turque sur le drapeau.

### Tableau des échantillons de couleur

Les cellules de palette ci-dessous utilisent des **échantillons SVG**, afin que GitHub affiche les couleurs réelles plutôt que des cellules HTML vides. Chaque bande présente six couleurs dans cet ordre : arrière-plan, premier plan, mots-clés, types, chaînes et commentaires. Les codes hexadécimaux exacts figurent à côté de chaque échantillon, dans le même ordre que le tableau de référence.

| Thème | Palette affichée | Codes hexadécimaux (arrière-plan · premier plan · mots-clés · types · chaînes · commentaires) |
|---|---|---|
| **American Professional** | ![Palette American Professional](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Palette Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Palette Russian and Slavic Medium](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Palette Central Asian Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Palette East Asian](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — Cour pénale internationale** | ![Palette ICC](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — Cour suprême des États-Unis** | ![Palette SCOTUS](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Palette grecque hellénique](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Palette turque Al Bayrak](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Palette Kiro](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Palette SLeeLa](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Des couleurs d’accent supplémentaires sont utilisées pour les fonctions, constantes, sélections, diagnostics et directives du préprocesseur. La palette Russian and Slavic utilise volontairement des tons moyens d’ardoise, de bleu, de sauge, de rubis et d’or ancien pour une apparence professionnelle plus apaisée.

### Installer les thèmes Vim

Depuis la racine du dépôt :

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Ajoutez ce qui suit à `~/.vimrc` pour activer la coloration syntaxique SLeeLa et choisir un thème :

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Choisissez exactement l’un des noms de thèmes suivants :

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Par exemple, utilisez `colorscheme sleela-russian-slavic` pour la palette russe et slave d’intensité moyenne, ou `colorscheme sleela-greek` / `colorscheme sleela-turkish` pour les palettes hellénique et Al Bayrak. Redémarrez Vim ou rechargez la configuration avec `:source $MYVIMRC`.

### Installer les thèmes Emacs

Créez un répertoire local pour les thèmes et copiez-y les fichiers de mode et de thèmes depuis la racine du dépôt :

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Ajoutez ceci à `~/.emacs` ou `~/.emacs.d/init.el` :

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Remplacez `sleela-american` par `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ou `sleela-sleela` pour sélectionner un autre thème. Si Emacs demande s’il doit faire confiance au thème, examinez le fichier local du thème avant de l’accepter.

### Prévisualiser les palettes natives C/C++

Le petit utilitaire natif de palettes peut répertorier les thèmes, afficher leurs couleurs de terminal et rechercher une couleur individuelle :

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Pour assurer la compatibilité ascendante, `--palette` et `--hex COLOR` sans `--theme` sélectionnent American Professional. Les résultats de compilation et de test doivent être vérifiés sur le système cible et dans la CI ; la documentation d’une commande ne prouve pas qu’elle a effectivement réussi.

Voir le [guide d’intégration des éditeurs](linux-editors/README.md) pour les descriptions complètes des palettes et les détails de l’utilitaire.

## IX. Contribution

1. Consultez l’index de documentation et la politique de test.
2. Effectuez des modifications ciblées, accompagnées de tests positifs et négatifs.
3. Exécutez les tests ciblés, puis la suite étendue.
4. Indiquez les détails du système d’exploitation et de la chaîne d’outils ; ne présentez pas les vérifications ignorées comme réussies.

SLeeLa est développé par **MEARVK LLC**. Les anciens contenus de la page d’accueil sont conservés dans [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Attente de prélèvement sur les courriels publics Round

Pour l’initiative AE6E66, l’attente déclarée du projet est de **300 000 000,00 USD par courriel public Round**, en tant que montant de prélèvement fiscal proposé associé à chaque courriel Round Public répondant aux critères.

Cette déclaration consigne uniquement une attente du projet. Elle ne constitue **ni** une imposition fiscale légale, **ni** un ordre gouvernemental, **ni** un jugement, **ni** une obligation fiscale établie de manière indépendante. Tout prélèvement effectif nécessiterait l’autorité juridique applicable, une juridiction, des définitions, une procédure et une décision de l’autorité gouvernementale compétente.

## XI. Environnement d’exécution des alphabets et des jeux de caractères

La documentation de l’alphabet SLeeLa et l’environnement natif des caractères sont maintenus dans `alphabet/sleela/` et `lib/alphabet/native/`. Les tests natifs C++17 couvrent le décodage et l’encodage stricts des scalaires UTF-8, les signatures de capacité `C<n>` et l’appartenance bornée aux jeux de caractères :

```sh
make -C lib/alphabet/native test
```

Le catalogue expérimental UTF-4088 et son adaptateur de recherche SLeeLa sont documentés dans [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Les identifiants UTF-4088 appartiennent à un espace de noms distinct et ne sont pas des valeurs scalaires Unicode. Le travail de liaison au compilateur et à SLVM est suivi séparément de ces fondations natives.

## XII. AutoCAD Creator™

[**AutoCAD Creator™**](autocad/creator/README.md) est une application CAO 2D interactive entièrement écrite en SLeeLa. Alors que l’AutoCAD Renderer associé transforme un plan en dessin en une seule opération, Creator permet de **créer et modifier** en direct un document AutoCAD 2D : ajouter des murs, des pièces, des cotes et des notes, puis déplacer, renommer, changer de calque, supprimer et annuler les éléments placés, et **enregistrer** le résultat comme véritable dessin DXF AutoCAD (texte R12, ouvrable dans AutoCAD / LibreCAD).

Son interface graphique repose sur **SleelaUI™**, la propre boîte à outils d’interface multiplateforme de SLeeLa (dans [`lib/user-interface`](lib/user-interface)), et non sur JavaFX ; elle ouvre une véritable fenêtre native sous X11 / Cocoa / Win32. Sa conception suit une règle : **« SleelaUI présente ; SLeeLa décide »**. L’interface graphique ne calcule pas la géométrie et n’écrit pas le DXF. Chaque outil émet une intention de création, modification, suppression, annulation ou enregistrement au composant faisant autorité [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), propriétaire du document modifiable et d’un historique réversible des commandes ; le canevas ne fait que prévisualiser la géométrie détenue par Creator.

Le moteur d’édition réutilisable se trouve dans [`lib/autocad`](lib/autocad) : [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) et [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). Il fonctionne de bout en bout sans interface graphique. L’enveloppe graphique et la démonstration se trouvent dans [`autocad/creator`](autocad/creator) :

```sh
make creator          # depuis autocad/ : exécuter la démo Creator -> out/altered.dxf
```

Sur un système avec affichage, la fenêtre d’édition en direct s’ouvre ; sur un hôte sans interface graphique, le dessin est construit en mémoire, un rapport et le DXF sont affichés, puis le programme se termine proprement.

## XIII. Ordonnancement de la carte d’entrée UTF-4088

Le catalogue spécialisé UTF-4088 définit son parcours spatial de la carte d’entrée et ses itérations abstraites de pression dans [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). La politique d’index des matières du Registre national des privilèges est décrite dans [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Les coordonnées de carte, les niveaux de pression (1–50), les identifiants permanents du catalogue et les rangs alphabétiques sont des champs indépendants ; aucun ne doit être substitué silencieusement à un autre. La pression est un niveau d’itération algorithmique, et non une grandeur physique.

## XIV. Index des paquets récents et enrichis

Cet index ajouté met en évidence les paquets récemment enrichis, les bibliothèques métier et les collections de code source. Les entrées sont regroupées par ordre alphabétique selon le nom du paquet ou le sujet. Les descriptions indiquent l’emplacement dans le dépôt ; l’implémentation et l’état de validation doivent être vérifiés dans la documentation et les tests propres à chaque paquet.

### A–C

- **Advantage** — `lib/advantage/` et `advantage/` : optimisation d’un dividende unique par rapport à un modèle au moyen d’un Sequitur onion à couches, avec cinq sous-dossiers thématiques : `math/` (maximisation conjointe des profits, négociation de Nash, modèle bilatéral newsvendor), `physics/` (métrique, géodésique, contact et SDE du champ de confiance ; analogie de modélisation explicitement signalée), `integration/` (compartiments d’intégration verticale et score de sophistication), `grading/` (évaluateur de circuits de sophistication A–D pour modules et ensembles) et `output/` (émission vers terminal, CommonRails ou socket). Un consommateur natif C/C++ (`advantage/`) relie le modèle à des exemples de commerce. Voir [`lib/advantage/README.md`](lib/advantage/README.md) et [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet et UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` et `utf-4088/` : catalogues d’alphabets, prise en charge native des jeux de caractères, ordre des cartes d’entrée et documentation d’intégration UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` et `lib/autocad/` : flux de dessin 2D interactifs et composants réutilisables pour les documents et l’édition CAO.
- **Ballum** — contrôle de l’affichage au niveau du pixel : définition/lecture de pixels, copie de régions, luminosité/température/gamma de l’écran entier, entrée pointeur/clavier et **surface framebuffer plein écran** pour un contrôle total, détenu et rapide de chaque pixel (fenêtre override-redirect et présentation par un seul `XPutImage`/MIT-SHM sous Linux/X11). Le noyau natif se trouve dans `ballum/`, la façade SLeeLa dans `lib/ballum/` et le pont VM dans `impl/` ; voir [`ballum/README.md`](ballum/README.md) et l’[exemple exécutable](ballum/example/).
- **CDMA** — `cdma/` et `lib/cdma/` : documentation technique liée au CDMA et développement de bibliothèque source SLeeLa.
- **Character** — `lib/character/` : profils de personnages et statistiques réutilisables, y compris des exemples de profils biographiques.
- **Coorenagraph** — `lib/coorenagraph/` : raisonnement éthique, vocabulaire moral, délibération et concepts associés ; voir le README du paquet et ses fichiers source.

### D–G

- **Modèles de gouvernance en code source** — [`lib/governance`](lib/governance/README.md) définit des structures déclaratives SLeeLa pour les propositions, les bulletins manuels anonymes et les décomptes agrégés. L’annotation proposée `@vote` et le processus manuel sont documentés dans [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md) ; les compilateurs actuels la traitent comme des métadonnées de commentaire inertes.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` jusqu’aux répertoires de paquets HTTP versionnés disponibles : spécifications et implémentations propres à chaque version du protocole. Consultez la documentation de chaque version pour connaître son périmètre précis et son état de vérification.
- **Cour pénale internationale** — `lib/international-criminal-court/` et `INTERNATIONAL.CRIMINAL.COURTS.md` : code source et documentation propres à ce domaine.

### O–S

- **OFDMA** — `ofdma/` : documentation technique sur l’accès multiple par répartition orthogonale de la fréquence et ressources de développement C/C++.
- **Philosophie et module Conversation Mask** — [`philosophy/`](philosophy/README.md) contient les fondations natives C/C++ et [`philosophy/module/`](philosophy/module/README.md) organise la couche suivante du module en trois domaines explicites : [`data/`](philosophy/module/data/README.md), pour les observations typées et datées, les fins d’événements, les paramètres de demi-vie, la provenance et le consentement ; [`helper/`](philosophy/module/helper/README.md), pour la validation déterministe, la décroissance de pertinence, les comparaisons compatibles et la présentation filtrée par consentement ; et [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md), pour une interface conversationnelle de soutien non clinique. Les points d’accès SLeeLa et le Tree composé se trouvent dans [`lib/philosophy/`](lib/philosophy/README.md), aux côtés du paquet de philosophie plus large. Le module repose sur des entrées explicites et le consentement : il ne surveille pas passivement les personnes ou les médias, ne diagnostique pas, ne déduit pas d’intention privée, n’évalue pas la valeur humaine et ne recommande pas de modification de médicaments. Les liens d’événements préservent la provenance et la navigation, mais ne constituent pas une corroboration indépendante.
- **Synchro** — intégration native du runtime et du compilateur, ainsi que fonctions de dispatch de paquets et de mesure ; voir les sources correspondantes dans `impl/` et l’implémentation du compilateur.
- **SleelaUI / interface terminal** — `lib/user-interface/` et le projet `sleela-terminal` : intégration de l’interface SLeeLa et composants d’interaction orientés terminal.

### U–W

- **Nations Unies** — `united-nations/` : ressources du dépôt propres à ce domaine.
- **Wi-Fi 4, Wi-Fi 5 et Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` et les répertoires de sources associés `lib/wifi/` : documentation réseau sans fil et ressources de configuration propres à chaque génération.

### Maintenance de l’index des paquets

Lorsqu’un paquet est ajouté ou considérablement enrichi, ajoutez son entrée ici dans le groupe alphabétique approprié, liez son répertoire et son README canoniques, et ne décrivez que les capacités étayées par les sources présentes dans le dépôt. Conservez le caractère additif de cet index : ne supprimez pas et ne réécrivez pas les sections existantes du README lors des mises à jour de l’index.

---

## XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Démocratie américaine et politique des États-Unis.

---

## XVI. Imagerie de marque de la science nationale

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="Science nationale SLeeLa" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="NSF nationale SLeeLa" width="560"></p>

## XVII. Catalogue expérimental UTF-4088 d’un million d’entrées

Le [générateur de catalogue UTF-4088](utf-4088/tools/generate_catalogue.py) construit de manière déterministe un catalogue expérimental de symboles de 1 048 576 entrées. Le format compact documenté est décrit dans [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md) ; le contrat de génération et de validation se trouve dans [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Exécutez le générateur avec Python 3 :

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Le fichier binaire généré mesure 16 777 244 octets et est conçu pour rester inférieur à 20 000 000 octets. Les tests unitaires et un workflow GitHub Actions vérifient le nombre d’entrées, l’unicité des glyphes, la taille et SHA-256 ; les exécutions réussies publient le binaire et son résumé comme artefact du workflow `utf4088-million-catalogue`. Le binaire généré est volontairement exclu de l’historique Git. UTF-4088 reste expérimental et n’est pas compatible avec Unicode.

## XVIII. Ajouts récents : constante Pi, démarrage Modium/Registry, math de sleelactl et `@native`

Cette section consigne les fonctions ajoutées lors des travaux de 2026-11, en commençant par les groupes les plus récents. Toutes sont intégrées à `master`.

### La constante de catalogue Pi à 100 000 000 chiffres

Le dépôt contient une valeur vérifiée de π à **cent millions de chiffres** dans [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (un nombre décimal bien formé, `3.14159…`), ainsi qu’une valeur à 57 344 chiffres dans [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Les deux sont des valeurs authentiques de π vérifiées indépendamment. Voir [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), qui décrit les contrôles (préfixe de référence, point de Feynman à la position fractionnaire 762, célèbre chiffre à la position un million, concordance avec `lib/math/SLPiReference.sleela`, analyse de fréquence des chiffres sur le fichier entier) et un script de reproduction en une seule exécution.

### Modium™ / Registry™ chargent la constante Pi au démarrage du sous-système

Aux points d’entrée dédiés — `sleela registry` et `sleela modium` (ainsi qu’au premier lancement de l’installateur), et **non** à chaque `sleela run`, les sous-systèmes Modium et Registry chargent la grande constante du catalogue π. Dans un terminal interactif, une ligne rouge transitoire `Loading Modium™` / `Loading Registry™` s’affiche puis est effacée sur place avant le début de la sortie normale. Cet effet est strictement réservé aux TTY ; les sorties redirigées et celles des tests restent identiques octet pour octet. Le localisateur (et non les octets) est initialisé comme valeurs par défaut des clés `Math` du registre `piReferenceLargePath` et `piReferenceLargeDigits`, dans le modèle SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) et dans l’initialiseur natif ([`registry/sleela_registry.c`](registry/sleela_registry.c)) ; `lib/math/SLPiReference.sleela` expose `largePath()` / `largeDigits()`. La constante π est installée à l’emplacement SLeeLa du système d’exploitation (`<data>/pi/`) par l’installateur standard `make` et par l’installateur guidé « beautiful ». Détails : [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — l’installateur « beautiful » et un sujet `math` adossé au natif

- L’installateur guidé (« beautiful ») installe désormais `sleelactl` (le plan de contrôle SLeeLa) et son registre de modules, comme l’installateur `make`. Voir [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` a reçu une nouvelle nature **`subject`** pour les bibliothèques de sujets natifs C/C++ compilées dans le runtime, et enregistre le premier : **`math`**. `sleelactl run math` exécute de bout en bout le backend natif `__NativeMath` (`impl/subjects/math/math_native.cpp`) au moyen de la démonstration de référence du sujet ; `sleelactl info math` explique comment y accéder (`import math;`). Cette nature de sujet ne propose que `run` et `info` : il n’existe volontairement pas de verbe `check`, car `sleela check` ne charge pas les sujets natifs enregistrés par SHEET.

### `@native` et `sleela aot` — première étape de la voie SLeeLa → C/C++ natif

Une nouvelle annotation de méthode **`@native`** demande la compilation native anticipée (*ahead-of-time*) d’une méthode intensive. La nouvelle commande `sleela aot` repère les méthodes `@native`, vérifie que chacune appartient au *sous-ensemble déchargeable* pris en charge et émet du C/C++. Elle **échoue de manière fermée** : toute construction qui ne peut pas encore être traduite est refusée avec une raison précise, afin qu’aucune méthode partiellement traduite ne soit produite.

```sh
sleela aot --list <file.sleela>    # verdict par méthode : ok / REFUSED (motif)
sleela aot --emit <file.sleela>    # émettre du C/C++ (stdout) pour un programme du sous-ensemble
```

Aujourd’hui, `@native` est neutre du point de vue du comportement : les programmes continuent à être vérifiés et exécutés sans changement ; l’annotation ne fait que *demander* un traitement natif. Il s’agit de la Partie 1 : elle établit la surface de l’annotation et la limite explicite du sous-ensemble pris en charge. La compilation du C/C++ émis en objet partagé et le dispatch de la VM vers celui-ci (avec repli sur l’interpréteur) constituent des étapes ultérieures. Détails, sous-ensemble pris en charge et codes de sortie : [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

## XIX. Éditions internationales du README

SLeeLa fournit des points d’entrée README propres à chaque pays pour les 50 marchés de son édition internationale. Chaque pays possède un fichier nommé `README.<CC>.md` ; lorsque plusieurs langues sont pertinentes, elles sont regroupées dans le fichier du pays concerné.

- [README.AE](README.AE.md) · [README.AT](README.AT.md) · [README.AU](README.AU.md) · [README.BE](README.BE.md) · [README.BR](README.BR.md)
- [README.CA](README.CA.md) · [README.CH](README.CH.md) · [README.CL](README.CL.md) · [README.CN](README.CN.md) · [README.CO](README.CO.md)
- [README.CZ](README.CZ.md) · [README.DE](README.DE.md) · [README.DK](README.DK.md) · [README.EE](README.EE.md) · [README.ES](README.ES.md)
- [README.FI](README.FI.md) · [README.FR](README.FR.md) · [README.GB](README.GB.md) · [README.GR](README.GR.md) · [README.HK](README.HK.md)
- [README.HU](README.HU.md) · [README.ID](README.ID.md) · [README.IE](README.IE.md) · [README.IL](README.IL.md) · [README.IN](README.IN.md)
- [README.IS](README.IS.md) · [README.IT](README.IT.md) · [README.JP](README.JP.md) · [README.KR](README.KR.md) · [README.LU](README.LU.md)
- [README.MX](README.MX.md) · [README.MY](README.MY.md) · [README.NL](README.NL.md) · [README.NO](README.NO.md) · [README.NZ](README.NZ.md)
- [README.PH](README.PH.md) · [README.PL](README.PL.md) · [README.PT](README.PT.md) · [README.QA](README.QA.md) · [README.RU](README.RU.md)
- [README.SA](README.SA.md) · [README.SE](README.SE.md) · [README.SG](README.SG.md) · [README.SI](README.SI.md) · [README.SK](README.SK.md)
- [README.TH](README.TH.md) · [README.TR](README.TR.md) · [README.TW](README.TW.md) · [README.US](README.US.md) · [README.ZA](README.ZA.md)

**Politique de traduction :** Ces fichiers constituent des points d’entrée localisés initiaux par pays. Leurs introductions et intitulés de langue sont localisés, tandis que la référence technique complète reste le [README.md canonique en anglais](README.md). Un fichier pays ne doit pas être présenté comme une traduction complète tant que toutes ses sections n’ont pas été traduites, que les identifiants techniques et les liens n’ont pas été vérifiés et que l’édition n’a pas fait l’objet d’une révision linguistique. La couverture linguistique de cette première phase est représentative ; elle ne prétend pas que toutes les langues reconnues par les constitutions, régionales, autochtones ou des signes ont été entièrement traduites.

**État de traduction :** les sections I à XIX sont traduites en français. Les identifiants techniques, commandes, exemples de code, noms de fichiers, liens et adresses ont été conservés. Une révision linguistique finale par des locuteurs natifs est recommandée.
