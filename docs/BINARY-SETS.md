# Building an OS-specific binary set

This guide is for people and AI coding agents (Kiro, Claude, Gemini, Codex, Copilot, and others) who need to pull the right folders from this repository and build a flat set of runnable executables for one operating system, as was done for
[`sleela-3.2-binaries`](https://github.com/mearvk/APML/tree/main/sleela-3.2-binaries) in the APML repository.

The machine-readable companion is [`binary-sets.json`](binary-sets.json). It lists every module that was built, the folders each one needs, the command, and the outputs. Read the JSON when you need to script a build. Read this file when you need the rules.

Status of this guide: **Linux x86-64 is built and verified. macOS and Windows are documented but were not built or tested in this work.** Do not claim a macOS or Windows binary set from this guide alone.

## 1. What a binary set is

A binary set is one flat directory that holds only runnable executables for one OS and CPU, built from one pinned commit of this repository, with a README that describes every file and a `SHA256SUMS` file.

It does not contain source folders, object files, static or shared libraries, test fixtures, installers or build trees.

## 2. Which documents exist, and why

| File | Audience | Purpose |
|------|----------|---------|
| `docs/BINARY-SETS.md` (this file) | People and agents | The rules and the procedure. |
| `docs/binary-sets.json` | Scripts and agents | Measured inventory: modules, folders needed, commands, outputs, failures. |
| `AGENTS.md` | Any agent | Short entry point with the hard rules and a pointer here. The cross-tool convention. |
| `CLAUDE.md`, `GEMINI.md` | Claude Code, Gemini CLI | One-line imports of `AGENTS.md`, because those tools read their own filename. |
| `.kiro/steering/binary-sets.md` | Kiro | Short steering file pointing here. |

Keep the facts in this file and the JSON. The others only point at them, so they cannot drift.

## 3. Hard rules

1. Publish executables only. No `.o`, `.a`, `.so`, `.dll`, `.dylib`, source folders, CMake probe files or fixtures.
2. Record the source commit and the toolchain in the set's README. Add `SHA256SUMS`.
3. Describe every binary. A set is not finished until each file has a row in the README and none is missing or invented (check this with a script).
4. Do not start server binaries to document them. Some open listeners at once. `skya` listens on `0.0.0.0:8443` the moment it runs. Read usage with `--help` only for tools you know are not servers, with a short timeout, in a scratch directory.
5. Do not run installers or any target whose dry run (`make -n`) writes to `/usr`, `/etc` or uses `sudo`. Skip `install/`, `jetbrains/`, `build/macos` and `build/windows` on Linux.
6. Do not regenerate `security/sha256-manifest.json` just to get past the integrity gate. See section 6.
7. Say what was not built and why. Never present an untested OS as supported.

## 4. Pulling the right folders

### Full checkout (simplest)

```sh
git clone --depth 1 https://github.com/mearvk/SLeeLa.git
```

### Sparse checkout (about 119 MB instead of the whole repository)

The core toolchain (`sleela`, `sleelvac`, `nordshrift`) needs these 19 top-level entries and nothing else. This set was found by building until nothing was reported missing, and the result was checked (section 5).

```sh
git clone --depth 1 --filter=blob:none --sparse https://github.com/mearvk/SLeeLa.git
cd SLeeLa
git sparse-checkout set --no-cone \
  /impl /runtime /os-level-support /security /http-servers /bash /common-rails \
  /audio /registry /preferred-routers /SHEET.sheet /tools /java28 /include \
  /idea /user-interface /wifi-common /ballum /ledger
```

Every other module needs its own folder plus the folders listed under `sparse_needs` for it in `binary-sets.json`. Most need only their own folder. The HTTP servers add `impl`, `preferred-routers` and `resolver`. The eleven SLVMs need the core set plus `sleela-virtual-machine` and a built `impl/build/libslcore.a`.

To add a module to a sparse tree:

```sh
git sparse-checkout add /antivirus
```

If a build stops with `No rule to make target '../X/...'` or `fatal error: ... .h: No such file`, the missing folder is the first path segment after the `..`. Add it and rebuild. That is how the lists in the JSON were produced.

## 5. Building (Linux, verified)

### Packages

Tested names are for Amazon Linux 2023 (dnf): `gcc gcc-c++ make python3 cmake libX11-devel fontconfig-devel freetype-devel libpng-devel zlib-devel openssl-devel`. The Debian and Ubuntu equivalents in the JSON (`build-essential`, `libx11-dev`, `libfontconfig1-dev`, `libfreetype-dev`, `libpng-dev`, `zlib1g-dev`, `libssl-dev`) were not tested.

### The core toolchain

```sh
export SLEELA_SHA256_MANIFEST=/path/to/a/manifest.json     # see section 6
cd impl
make -j8 build/sleela build/sleelvac build/nordshrift
```

With the sparse set above, this produced byte-identical binaries to the published ones (same compiler and flags):

| Binary | SHA-256 |
|--------|---------|
| `sleela` | `e56dd85b4ca28f9967fb84d3942dcf8f7ad3acfd913a9d02ec73e9f839fac7cd` |
| `sleelvac` | `c07825dde653517280b49b7f9349be4feba68f5e9f2eaae3815f5e6a9d433811` |
| `nordshrift` | `3963b391f168eac2e936c484df1654db43ae77d3d0dde2a8c645b6f429616e7d` |

A different compiler or version will give different hashes. That is expected.

### Other modules

Each module is built from its own directory with its default target, using the command in the JSON:

```sh
make -C <build_dir> -j8 all          # most modules
make -k -j8 all                       # sleela-terminal: -k keeps going past the GTK4 app
make -C impl libslcore && make -C sleela-virtual-machine/001 all    # SLVMs
cmake -S <dir> -B <out-of-tree> -DCMAKE_BUILD_TYPE=Release && cmake --build <out-of-tree> -j8   # CMake projects
```

Before running a module, check `make -n` for system-path writes (rule 5).

## 6. The integrity gate

`impl/Makefile` has an order-only prerequisite, `verify-security`, that checks every file in `security/sha256-manifest.json` and fails closed on any mismatch or any missing file.

Two consequences:

- **At the time this guide was written the committed manifest did not match 19 of its own source files.** The build stopped until a manifest was supplied that matches the tree. The binaries in `sleela-3.2-binaries` were built against a manifest generated from the same checkout and kept outside the repository, so the committed manifest was not changed.
- **A sparse checkout cannot use the committed manifest at all**, because it lists files that are not present. You must supply a manifest generated from the sparse tree.

Do this, from the checkout root:

```sh
python3 tools/generate-sha256-manifest.py --root . --output /tmp/manifest.json   # outside the repo
export SLEELA_SHA256_MANIFEST=/tmp/manifest.json
```

This checks that the tree has not changed since you generated the manifest. It does **not** prove the tree matches a trusted commit. For that, use a full checkout of a commit whose committed manifest verifies (`cd impl && make check-manifest`), and say in the README which of the two you did. If you intentionally edited verified sources, review the diff and use `make regen-manifest` as `impl/Makefile` describes, then commit the new manifest with the sources.

Binaries built against a regenerated manifest refuse to run in a stock checkout until its manifest is regenerated or `SLEELA_SHA256_MANIFEST` points to a matching one. Put that in the set's README.

## 7. Collecting the set

1. Make a timestamp file before building: `touch /tmp/stamp`.
2. After building, list new files that are executable ELF files (first four bytes `7f 45 4c 46`), newer than the stamp. Skip `*.so`, `*.so.*`, `*.o` and anything under `CMakeFiles/`.
3. Drop duplicates (same SHA-256).
4. Copy into one flat directory. If two different binaries share a name, rename both so the name says where each came from (the APML set uses `sleelas-api-2.1` and `sleelas-server-edition`, `heritage-print-test-c` and `-cpp`).
5. Check dependencies: `ldd ./<file> | grep "not found"` must print nothing.
6. Write `SHA256SUMS` and the README with one row per file. Check coverage by script: the set of files must equal the set of names in the README tables.

Put the set where it will be consumed. In this work that was the APML repository, in `sleela-3.2-binaries/`. Remember that `**/build/` is ignored in this repository's `.gitignore`. Do not commit build outputs here. Force-add (`git add -f`) only the small source files that are meant to be tracked under `build/`.

## 8. macOS and Windows

Entry points already exist and are documented:

| OS | Format | Build entry point | Notes |
|----|--------|-------------------|-------|
| macOS | Mach-O | `make -C build/macos` | Apple Clang, Xcode Command Line Tools, Homebrew OpenSSL 3. See `build/macos/README.md`. |
| Windows 10+ | PE (`.exe`) | `make -C build/windows` or `build-windows.ps1` | MSYS2 UCRT64 MinGW-w64, not MSVC. See `build/windows/README.md` and `docs/PLATFORM-SUPPORT.md`. |

What changes in the procedure: detect executables by their format (Mach-O magic `feedface`, `feedfacf`, `cafebabe`, or `cffaedfe`; PE files start with `MZ` and end in `.exe`), because the executable bit is not a reliable signal on Windows. Everything else (rules, flat layout, README, checksums) stays the same.

Until someone builds and runs a set on those systems, label it unverified.

## 9. What did not build

The JSON lists these under `not_built` with reasons. In short: modules that need system development packages (nghttp2, GTK4, VTE, kernel headers, database client libraries), modules with source defects (`http-1.0`, `sommeliers`, `server-edition/http`), and SLeeLa programs whose targets run checks and not native builds. Installers are excluded by policy.

## 10. Checklist before publishing a set

- [ ] Built from a recorded commit; toolchain noted.
- [ ] Only executables; no libraries, objects or source folders.
- [ ] Every file has a README row, and the README has no extra rows (scripted check).
- [ ] `ldd` shows no missing libraries.
- [ ] Servers were not started to describe them.
- [ ] The integrity-gate method is stated (committed manifest verified, or generated manifest).
- [ ] Not-built modules are listed with reasons.
- [ ] Any OS other than Linux is labelled unverified unless it was built and run there.
