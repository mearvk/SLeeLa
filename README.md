# SLeeLa
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>A native systems language, compiler, runtime, and virtual-machine ecosystem.</strong></p>
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
- [Recursion stanchion — C and C++ prototypes](recursion/stanchion/README.md) — primitive-specialized examples for Boolean, Byte, Character, Integer, Long, Float, and Double, alongside generic, applied, scientific, and teaching recursion patterns.
- [Platform support and validation matrix](docs/PLATFORM-SUPPORT.md)
- [Brand guide](docs/BRAND-GUIDE.md)
- [Merchandise concepts and assets](merchandise/README.md)
- [NitroWebExpress logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testing policy and release gates](markdown/TESTING.md)

## II. News

Recent additions to the language, drivers, and standard library:

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
  **`#sleela 2.8`**. See [`man/man1/sleela.1`](man/man1/sleela.1) and
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
  128-bit a documented follow-up). See
  [`SLEELA.syntax`](SLEELA.syntax) §21.
- **Document markers — the Natural Process (`author`/`authorial`, `material`,
  `memory`, `determine`, `object`, `field`).** First-class, declarative-only
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
- **Nordshrift at syntax 2.5 with full library reach.** Nordshrift resolves
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
[registry guide](registry/README.md).

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
The authoritative grammar is [`SLEELA.syntax`](SLEELA.syntax). The current repository documents syntax **2.6**; compiler/tool version and source syntax version are distinct. The 2.6 surface adds **keyword supporter documents** (§22) — every use of `try`/`catch`/`throw`/`synchronized`/`super` is hashed, recorded as a `@supporter(...)` annotation, and linked to a `docs/keyword-supporters/` folder the developer fills out — and the **`determine` document marker** (§23), a declarative-only sibling to `material`/`memory`/`author`. It carries forward the wide signed integer primitives `long`/`int64`/`int128` (§21) and the declarative document markers `material`/`memory`/`author`/`authorial` (§18–§20). Constructor overload resolution and `this(...)`/base-constructor delegation are explicitly outside the verified syntax 1.8 contract. Consult the grammar and implementation notes before relying on a feature.

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

- **Alphabet and UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/`, and
  `utf-4088/`: alphabet catalogues, native character-set support, input-map
  ordering, and UTF-4088 integration documentation.
- **AutoCAD Creator** — `autocad/creator/` and `lib/autocad/`: interactive
  2D drawing workflows and reusable CAD document/editing components.
- **Ballum** — SleelaUI event and widget integration, including VM bridge support
  under `impl/`; see the corresponding implementation and API documentation.
- **CDMA** — `cdma/` and `lib/cdma/`: CDMA-related technical material and
  SLeeLa source-library work.
- **Character** — `lib/character/`: character profiles and reusable character
  statistics, including biographical profile examples.
- **Coorenagraph** — `lib/coorenagraph/`: ethical reasoning, moral vocabulary,
  deliberation, and related concepts; see its package README and source files.

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
