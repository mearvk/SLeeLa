# SLeeLa 3.0 — South Africa (ZA)

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

## I. Start here

- [Build the native implementation](impl/README.md)
- [Virtual-machine and Modium user guide](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [CPU architecture in SLeeLa source](lib/cpu/CPU.md) — more than 130 CPU profiles; see [timing controls](lib/cpu/TIMING.md).
- [Documentation index](docs/README.md)
- [Generated API documentation](docs/api/README.md) and [API catalogue](api/README.md)
- [Platform support and validation](docs/PLATFORM-SUPPORT.md)
- [Testing policy](markdown/TESTING.md)

## II. Build and test

Requirements include a C/C++ toolchain (GCC or Clang), GNU Make, and Python 3. Linux graphical components may also require X11/Xrandr development packages.

Run from the repository root:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

A successful build of one component does not establish that every library or platform is release-ready. Consult the build and platform documentation for target-specific requirements.

## III. Install

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

The conventional system prefix is generally `/usr/local`. Setting `PREFIX="$HOME/.local"` provides a user-local installation and normally avoids administrator privileges. See the [installation guide](install/README.md) for details.

## IV. Language and runtime

The normative grammar is [`SLEELA.syntax`](SLEELA.syntax). This repository documents syntax **3.0**. Compiler version and source-language syntax version are distinct.

Technical references cover `@mix` contiguous fields, extended integer families, Preonem document markers, SleelaDoc, API documentation generation, and Nordshrift. Technical identifiers, commands, filenames, API names, and code examples remain unchanged.

## V. Engineering and validation

SLeeLa emphasizes evidence, explicit component contracts, portability, safe failure behavior, and reproducible releases. Validation must be assessed by module and platform. Skipped checks must not be reported as passing; one successful component build is not full release validation.

Sleelavirin protection tooling includes a local SHA-256 verifier, an optional ClamAV adapter, and an integrity manifest. A release should be considered validated only after its commit's CI checks finish and the resulting artifacts are reviewed.

## VI. Editors and contributions

SLeeLa provides **Vim** and **Emacs** themes and syntax integration, plus a native C/C++ palette utility. See the [editor integration guide](linux-editors/README.md).

Contributions should be focused, include positive and negative tests, and state the operating system and toolchain used. Do not describe skipped checks as successful.

SLeeLa is developed by **MEARVK LLC**.

## VII. Other technical areas

The main README also covers the Advantage, SleelaDoc, and XML DOM libraries; primitive integer types; the character runtime; AutoCAD Creator™; the experimental UTF-4088 catalogue; Pi, Modium, Registry, and `sleelactl` tools; and the initial native C/C++ compilation path enabled by the `@native` marker.

## Complete technical reference

The [canonical README](README.md) contains the complete feature catalogue, language and runtime reference, build and installation instructions, API links, examples, and release notes. This South African edition expands the practical English guide and retains the multilingual introductions, but it is not a complete line-by-line translation of the canonical technical README. English remains the normative technical source until reviewed translations are available.

- [Canonical README](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**Translation status:** Practical technical guidance is expanded in English. Full technical translations into the listed South African languages, including South African Sign Language resources, remain outstanding and should be reviewed by fluent technical contributors.
