# SLeeLa
<p align="center"><img src="images/brand/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/merchandise/sleela-marketing-logo-001.png" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>A native systems language, compiler, runtime, and virtual-machine ecosystem.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

**The Now. The Ingredient.** A language is more than syntax: it is the compiler, runtime, libraries, tools, and platform contracts that make software usable.

## Start here
- [Build the native implementation](impl/README.md)
- [Virtual machines & Modium — hands-on user guide](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU stack in SLeeLa source](lib/cpu/CPU.md) — a full gates→ALU→cores→OS→programs machine, a roster of 130+ CPU profiles, and adjustable [voltages, variable clocks, and expert timings](lib/cpu/TIMING.md) you can [run at spec and overclock](lib/cpu/components/README.md)
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

The repo-root `make all` builds every product (core, VM, codecs, churn,
antivirus, …) and ends by building the **registry** and materializing its
baseline table — so a built tree ships a reviewable registry out of the box:

```sh
make all                 # full build; the `registry` target runs last
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
```

The `registry` target runs [`build-registry.sh`](build-registry.sh): it builds
the registry core, seeds the baseline table into the portable text store
`registry/build/sleela.sleelareg`, and exports the MySQL table script
`registry/build/registry.sql` (via `sleela registry export`). Review it with
`sleela registry` or `registry/build/sleela-registry review`. See the
[registry guide](registry/README.md).

Consult the build and testing guides for prerequisites and platform-specific steps. A successful build of one component does not prove every library or platform is release-ready.

## Install

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
untracked runtime data. For the interactive deployment workflow and all
installation options, see [Quick and Safe Install](install/README.md).

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


## SLeeLa editor themes — Vim and Emacs

SLeeLa includes a coordinated set of professional dark color schemes for **Vim** and **Emacs**, plus a native C/C++ palette utility. The original **American Professional** scheme remains the default. Four companion palettes are available for different cultural design inspirations; these are interpretive design palettes, not official representations of any nation or culture.

### The five schemes and their colors

| Scheme | Background | Foreground | Keywords | Types | Strings | Comments |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russian and Slavic — Medium** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Central Asian / Silk Road** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **East Asian** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |


### Color swatch table

The palette cells below use **embedded SVG swatches**, so GitHub's HTML sanitizer cannot turn the colors into empty cells. Each strip renders the actual six colors—background, foreground, keywords, types, strings, and comments—in the same scheme order as the reference table above. The swatches themselves have rounded corners; the hex codes remain in the reference table above.

| Scheme | Rendered palette colors |
|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/american-professional.svg) |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/pan-eurasian.svg) |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/russian-slavic-medium.svg) |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/central-asian-silk-road.svg) |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/east-asian.svg) |

Additional accent colors are used for functions, constants, selections, diagnostics, and preprocessor directives. The Russian and Slavic palette intentionally uses medium-intensity slate, blue, sage, ruby, and antique gold for a calmer professional appearance.

### Install the Vim schemes

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

For example, use `colorscheme sleela-russian-slavic` for the medium Russian and Slavic palette. Restart Vim or source your configuration with `:source $MYVIMRC`.

### Install the Emacs schemes

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

Replace `sleela-american` with `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, or `sleela-east-asian` to select another scheme. If Emacs asks whether to trust the theme, inspect the local theme file before accepting it.

### Preview the native C/C++ palettes

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
