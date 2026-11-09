# SLeeLa 3.0 — Singapore (SG)

> **Singapore multilingual edition.** This page provides introductory and practical project guidance in English, Simplified Chinese, Bahasa Melayu, and Tamil. Technical identifiers, source code, commands, filenames, and API names remain unchanged.

## English

SLeeLa is a native systems programming language, compiler, runtime, and virtual-machine ecosystem. It brings together the language, compiler, libraries, developer tools, and platform contracts needed to build and run software.

## 简体中文

SLeeLa 是一套原生系统编程语言、编译器、运行时和虚拟机生态系统。它将语言、编译器、函式库、开发工具及平台契约整合在一起，用于构建和运行软件。

## Bahasa Melayu

SLeeLa ialah bahasa pengaturcaraan sistem natif yang merangkumi pengkompil, persekitaran masa jalan dan ekosistem mesin maya. Bahasa, pengkompil, pustaka, alat pembangunan dan kontrak platformnya bekerjasama untuk membina serta menjalankan perisian.

## தமிழ்

SLeeLa என்பது ஒரு இயல்பான கணினி அமைப்பு நிரலாக்க மொழி; இதில் தொகுப்பி, இயக்கநேரச் சூழல் மற்றும் மெய்நிகர் இயந்திரச் சூழல் அடங்கும். மொழி, தொகுப்பி, நூலகங்கள், உருவாக்குநர் கருவிகள் மற்றும் தள ஒப்பந்தங்கள் இணைந்து மென்பொருளை உருவாக்கவும் இயக்கவும் உதவுகின்றன.

## I. Start here / 开始使用 / Bermula / தொடங்குதல்

- [Build the native implementation](impl/README.md)
- [Virtual machines and Modium user guide](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [CPU architecture in SLeeLa source](lib/cpu/CPU.md) — more than 130 CPU profiles; see [timing controls](lib/cpu/TIMING.md).
- [Documentation index](docs/README.md)
- [Generated API documentation](docs/api/README.md) and [API catalogue](api/README.md)
- [Platform support and validation](docs/PLATFORM-SUPPORT.md)
- [Testing policy](markdown/TESTING.md)

## II. Build and test

Requirements include a C/C++ toolchain (GCC or Clang), GNU Make, and Python 3. Linux graphical components may also require X11/Xrandr development packages.

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

The conventional system prefix is generally `/usr/local`. Setting `PREFIX="$HOME/.local"` provides a user-local installation and normally avoids administrator privileges. For the guided installation flow and package details, see the [installation guide](install/README.md).

## IV. Language and runtime

The normative grammar is [`SLEELA.syntax`](SLEELA.syntax). This repository documents syntax **3.0**. Compiler version and source-language syntax version are distinct. Technical references cover `@mix`, extended integer families, Preonem document markers, SleelaDoc, API generation, and Nordshrift. Identifiers and code examples are retained verbatim.

## V. Engineering and validation

SLeeLa emphasizes evidence, explicit component contracts, portability, safe failure behavior, and reproducible releases. Validation must be assessed by module and platform. Skipped checks must not be reported as passing, and a single successful component build is not full release validation.

## VI. Editors and contributions

SLeeLa provides **Vim** and **Emacs** themes and syntax integration, plus a native C/C++ palette utility. See the [editor integration guide](linux-editors/README.md).

Contributions should be focused, include positive and negative tests, and state the operating system and toolchain used. Do not describe skipped checks as successful.

## Complete technical reference

The [canonical README](README.md) contains the full feature catalogue, language and runtime reference, build and installation instructions, API links, examples, and release notes. This Singapore edition expands the country-language introductions and essential practical guidance; it is not yet a complete line-by-line translation of every technical section. The English README remains the normative source until full translations receive technical review.

- [Canonical README](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**Translation status:** The four language introductions are provided above. Practical guidance is available in English, while full technical translations into Simplified Chinese, Malay, and Tamil remain outstanding.
