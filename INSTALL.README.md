# Installing SLeeLa

This document describes how to install SLeeLa and, in particular, how the
installer is **built with module-selection options** so you decide which
standard-library packages are staged — without being prompted interactively at
install time.

The installer produces three commands — `sleela` (runtime), `sleelvac`
(compiler), and `nordshrift` (the `.sst` → Java/SLeeLa/C transpiler) — plus the
selected standard-library packages, the integrity manifest, docs, and examples.

---

## Quick start

All conventional installation runs through the `install/` Makefile:

```sh
# User-local install (no sudo). Installs every standard-library package.
make -C install install-local

# System-wide install (uses sudo for system paths only).
make -C install install-system

# Scope-aware entry point (local prefix stays local; system prefix elevates).
make -C install install

# Refresh EVERY standard location in one run
# (user-local layouts + /usr/local + any present extra bindirs).
make -C install install-all

# Show all targets and options.
make -C install install-help
```

Each variant first offers to (re)generate and verify the base-code SHA-256
integrity manifest. Answer non-interactively with
`SLEELA_MANIFEST_CONSENT=yes|no|skip`.

---

## Module-selection options (built into the installer)

Which standard-library packages get installed is chosen **when you run the
build**, via make variables — not by an interactive prompt. The installer
artifact is produced with these options baked in.

| Option | Meaning |
| --- | --- |
| `MODULES=all` | Install **every** `lib/` package. This is the **default**. |
| `MODULES="pkg1 pkg2 …"` | Install **only** the named packages (the "individually" selection expressed as data). |
| `EXCLUDE_MODULES="pkgA pkgB"` | Drop these packages from whatever `MODULES` selected. |

### Examples

```sh
# All modules (default) — nothing extra needed.
make -C install install

# Only a chosen subset.
make -C install install MODULES="crypto codecs compression regex"

# Everything except a couple of trees.
make -C install install MODULES=all EXCLUDE_MODULES="ide wifi"

# Works on every install path and for staged (DESTDIR) package builds, e.g.:
make -C install install-local MODULES="crypto regex"
make -C install install-all   MODULES="crypto codecs regex"
make -C install do-install    DESTDIR=/tmp/pkgroot PREFIX=/usr/local \
    MODULES="crypto codecs" EXCLUDE_MODULES="debugger"
```

### Behavior notes

- **Default is unchanged.** With no `MODULES` given, the installer stages the
  full standard library exactly as before.
- **Selection is honored end to end.** `do-install` stages the library through
  `install-runtime.py stage-lib`, and the SHA-256 integrity restore
  (`copy-manifest`) is filtered to the selected packages, so omitted modules are
  **not** silently re-populated. Non-library core/tools/security files are
  always restored.
- **Unknown package names** are reported and skipped; they do not abort the
  install.
- **The `lib/*.md` inventory files** always travel with the library regardless
  of selection.
- **Uninstall stays correct.** The install tracking manifest
  (`.sleela-install-manifest.json`) records only the packages that were
  actually installed, so `make -C install uninstall` / `uninstall-all` remove
  exactly those.

### Finding package names

Package names are the top-level directories under `lib/`:

```sh
ls -d lib/*/ | sed 's|lib/||; s|/||'
```

---

## Install locations

- **User-local** (recommended, never uses sudo): installs to both
  `~/.local` and `~/.local/sleela` so the commands are present wherever a user
  expects them. Override with `PREFIX=...`.
- **System-wide**: defaults to `/usr/local` (binaries in `/usr/local/bin`,
  runtime under `/usr/local/lib/sleela`, config in `/etc/sleela`). Uses sudo for
  the privileged copy into system locations only.

`BINDIR`, `LIBDIR`, `DATADIR`, `SYSCONFDIR`, and `EXTRA_BINDIRS` may all be
overridden. See `make -C install install-help` for the full list.

---

## Prerequisites

Building the three core products requires a C/C++ toolchain and the native UI
dependencies used by the runtime:

- `g++` (C++17)
- `python3` (used for staging, manifest, and uninstall bookkeeping)
- X11, fontconfig, and freetype development libraries

If the compiled products are missing, build them first:

```sh
make -C impl all
```

---

## Uninstalling

```sh
# Remove a single tracked install at a prefix.
make -C install uninstall            [PREFIX=/usr/local]

# Remove from every standard location (mirror of install-all).
make -C install uninstall-all
```

Configuration directories are intentionally preserved; remove them manually if
you no longer need them.

---

## See also

- `install/README.md` — fuller installer reference.
- `install/Makefile` — the conventional installation targets and module knobs.
- `install/install-runtime.py` — safe staging, manifest, and uninstall logic.
- `install/sleela-clean-install.sh` — the calm, styled clean-install experience
  for the main package.
