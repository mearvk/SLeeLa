<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Quick and Safe Install

The installer supports user-local and system-wide installation on Linux, macOS, and Windows 10+. User-local installation is the recommended default and does not require administrator access.

## Clean Install (recommended for most users)

For a calm, beautiful, guided install of the **main package only**, run:

```sh
make clean-install          # or: ./install/sleela-clean-install.sh
```

It installs the binaries (`sleela`, `sleelvac`, `nordshrift`), the standard
library, protocols, and support modules — and **excludes** servers, IDEs/editors,
Wi-Fi implementations, and religious/moral/political systems. You make **one
primary decision** (install the main package), choose a scope, and then — on
success — may optionally add curated **enhancement modules** (audio, video,
signal-processing, codecs, compression, gui, ui, database, connector).

The output is styled with the SLeeLa palette and degrades to plain text
automatically when not attached to a terminal, when `NO_COLOR` is set, or with
`--plain`. The full-featured installer below (with server/HTTP/VM-edition
options) remains available for advanced deployments.

## Installation scope

At the start, choose:

- **User-local (recommended):** Linux/macOS default `$HOME/.local/sleela`; Windows default `%LOCALAPPDATA%\\SLeeLa`. The installer places compiled executables in the installation's `bin/` directory and can configure only the current user's PATH.
- **System-wide:** Linux/macOS default runtime root `/usr/local/lib/sleela`, executables exposed through `/usr/local/bin`, and startup configuration in `/etc/sleela/startup.conf`. The installer uses `sudo` for privileged writes. Windows default is `%ProgramFiles%\\SLeeLa`, requires an elevated PowerShell, and adds the install's `bin/` directory to the machine PATH.

The Linux/macOS user-local destination can be overridden with `SLEELA_INSTALL_ROOT`; system runtime root can be overridden with `SLEELA_SYSTEM_ROOT`. On Windows, pass `-Destination` to choose the destination explicitly.

## Installation flow

1. Choose user-local or system-wide scope.
2. Select SLeeLa System/server, HTTP servers 1–9, VM edition, and Port Authority startup options.
3. Build SLeeLa before deployment unless the Windows `-NoBuild` option is used.
4. Stage the runtime, including the compiled `sleela`, `sleelvac`, and `nordshrift` products.
5. Back up an existing destination before updating it.
6. Install binaries, libraries, and `config/startup.conf`.
7. Configure only the PATH scope corresponding to the selected installation mode.
8. Report the installation and configuration locations.

A missing compiled product or failed build stops installation before deployment.

## Platform entry points

- Linux: `./install/quick-safe-install.sh`
- macOS: `./install/quick-safe-install-macos.sh`
- Windows 10+: `install\\quick-safe-install.cmd` or `install\\quick-safe-install.ps1`

## Safety model

- User-local mode never elevates privileges or changes the machine PATH.
- System-wide mode explicitly requests administrator privileges and uses conventional system paths.
- Build failure stops deployment. Existing destination files are backed up before replacement.
- System-wide PATH is not changed on Linux/macOS because `/usr/local/bin` is the conventional executable location; `/etc/profile.d/sleela.sh` sets `SLEELA_HOME`. Windows system-wide mode adds its binary directory to the machine PATH.

## Commands

```text
make install-help
make install-check
make install-local                      # user-local, never uses sudo
make install-system                     # system-wide, uses sudo (system paths only)
./install/system-install.sh             # the sudo system installer directly
./install/quick-safe-install.sh
./install/quick-safe-install-macos.sh
install\\quick-safe-install.cmd
```

### User-local vs system-wide, and sudo

The two scopes are deliberately separated so a user-local install never has any
reason to elevate:

- **`make install-local`** installs for the current user and **never uses
  `sudo`**. With no `PREFIX` given it installs to **both** conventional
  user-local layouts, creating any missing directories on first install and
  requiring no environment variables:
  - `$HOME/.local` (FHS-style: `$HOME/.local/bin`, `$HOME/.local/lib/sleela`, …)
  - `$HOME/.local/sleela` (self-contained: `$HOME/.local/sleela/bin`, …)

  Passing an explicit `PREFIX=...` installs to that single prefix only. It
  refuses a system prefix (pointing you at `install-system`) and refuses to run
  as root.
- **`make install-system`** performs the system-wide install through
  [`system-install.sh`](system-install.sh). It runs the integrity step as the
  invoking (unprivileged) user first, then uses `sudo` **only** for the
  privileged copy into system locations.
- **`make install`** remains scope-aware: run with no `PREFIX` as a normal
  user it installs to **both** user-local layouts (`$HOME/.local` and
  `$HOME/.local/sleela`, same as `install-local`); a system prefix (with no
  `DESTDIR`) elevates automatically for the system copy only; an explicit
  `PREFIX=...` or a `DESTDIR` staged build installs to that single prefix.
- **`make install-all`** refreshes **every** standard SLeeLa location in one
  command, so a single run updates all the directories `whereis sleela`
  reports. Run it as your normal user (it invokes `sudo` itself for system
  paths). It performs, in order:
  1. the user-local **dual** install (`$HOME/.local` and `$HOME/.local/sleela`);
  2. the system install at **`/usr/local`** (sudo, system paths only);
  3. an **in-place refresh** of any `sleela`/`sleelvac`/`nordshrift` already
     present in `EXTRA_BINDIRS` (default `/usr/bin`) so stray copies never go
     stale. It never *creates* a new stray command — it only overwrites one that
     already exists. Override the set with `EXTRA_BINDIRS="/usr/bin /opt/bin"`.

  `install-all` and `uninstall-all` resolve the invoking user's home via
  `$SUDO_USER` when elevated, so the `$HOME/.local` layouts are found whether
  you run them as the user or under `sudo`.

  If a location already holds an **untracked** SLeeLa install (binaries but no
  `.sleela-install-manifest.json` — e.g. one left by `quick-safe-install.sh`),
  a plain per-prefix install refuses to clobber it. `install-all` passes
  `SLEELA_INSTALL_FORCE=1` so it **adopts** such an install — overwrites the
  binaries and starts tracking it — instead of aborting. You can set
  `SLEELA_INSTALL_FORCE=1` yourself on a single-prefix `install`/`do-install`
  to get the same adopt-and-overwrite behavior.
- **`make uninstall-all`** is the mirror of `install-all`: it removes the
  tracked SLeeLa install from **every** standard location — the user-local dual
  layout (`$HOME/.local` and `$HOME/.local/sleela`), the `/usr/local` system
  prefix, and any stray binaries in `EXTRA_BINDIRS` — skipping any location that
  has no tracked install. Prefer it over plain `uninstall` when SLeeLa is
  installed in more than one place. (Plain `make uninstall` removes a single
  prefix only, which is why it can leave copies behind in other locations.)

### Base-code integrity prompt on every install

Before any install variant copies files, it offers to **(re)generate and verify
the base SLeeLa code SHA-256 manifest** — the integrity baseline the runtime's
execution gate checks against — as a plain `yes/no` prompt:

- **yes** — regenerate the manifest from the current source tree, then verify.
- **no** — keep the committed manifest and verify the tree against it.
- **skip** — do neither for this install.

Answer non-interactively (CI, package builds) with
`SLEELA_MANIFEST_CONSENT=yes|no|skip`. When no terminal is attached and the
variable is unset, the step is skipped safely.

### How installed commands find their libraries (standard resolution)

You are expected to run the **installed** `sleela`/`sleelvac`, not the copy in a
git checkout. An installed command finds its support files from the **install
location**, derived from the command's own path, so it works from any working
directory and without setting environment variables:

- command: `<prefix>/bin/sleela`
- runtime root: `<prefix>/lib/sleela` — holds the `lib/` source-library tree,
  the `security/sha256-manifest.json` integrity manifest, the
  `tools/verify-before-execution.py` gate, and `SHEET.sheet`.

Resolution order for each resource (first match wins):

1. the matching environment variable, if set — `SLEELA_LIB` (library tree),
   `SLEELA_SHEET` (catalog), `SLEELA_HOME` / `SLEELA_SHA256_MANIFEST` (verify
   gate) — these remain available as explicit overrides;
2. the installed runtime root derived from the executable's own location
   (`<prefix>/bin` → `<prefix>/lib/sleela`);
3. the SLeeLa root found by searching upward from the current directory — this
   is what lets a developer run the build-tree binary from inside the checkout.

So after `make install` / `make install-system`, running `sleela run foo.sleela`
from anywhere resolves `/lib`, the manifest, and the catalog from
`<prefix>/lib/sleela` — not from the clone, and not from the current directory.
The clone is only used when you run the uninstalled build-tree binary.

### Reinstall safety: existing files are backed up, not clobbered

A reinstall over an existing install first **snapshots** the previous binaries
and the runtime/data trees into a timestamped directory beside the runtime tree
(`<prefix>/lib/sleela-backups/<YYYYmmdd-HHMMSS>/`) before writing the new files.
Old files are **retained there and never removed automatically** — delete a
snapshot yourself when you no longer need the rollback. The install tracker does
not record the backups, so `make uninstall` leaves them in place. (Override the
location with `BACKUP_ROOT=...`; staged `DESTDIR` package builds skip the backup
because they target a fresh package root, not a live system.) The installer also
still refuses to overwrite same-named commands that are not tracked as a SLeeLa
install.

### What `sudo` does and does not touch

When `make install-system` (or a system-scoped `make install`) elevates, `sudo`
is used **only to copy SLeeLa into system locations as the trusted system
account**. The install:

- does **not** change the owner, group, or permissions of files you already own;
- does **not** modify anything in your home directory;
- only writes **new** files under the system prefix, owned by the system account
  (root), as is conventional for `/usr/local`.

A confirming privilege note is printed at the end of a system install.

**SLeeLa — MEARVK LLC — 2026**

## Conventional `make install`

The conventional installation target is non-interactive and installs the built
native commands (`sleela`, `sleelvac`, and `nordshrift`) plus runtime libraries, verification tooling, documentation,
examples, and a default configuration. It does not replace the interactive
Quick and Safe installer above.

From the repository root:

```sh
make -C impl all
make install-local                        # user-local; NEVER uses sudo (defaults PREFIX=$HOME/.local)
make install-system                       # system-wide; uses sudo for system paths only
make install                              # scope-aware: local prefix stays local, system prefix elevates
make install PREFIX="$HOME/.local"         # user-local; stays as current user
make install DESTDIR="$PWD/package-root"  # staged package installation
make install SLEELA_MANIFEST_CONSENT=no   # answer the SHA-256 manifest prompt non-interactively
make install-check                        # verify installer entry points
make uninstall                            # remove tracked executables/runtime; preserve config
make uninstall PREFIX="$HOME/.local"       # remove a tracked user-local install
make -C install uninstall                  # lower-level equivalent
```

Privilege handling is scope-aware and split into two explicit targets so the
user-local path never elevates:

- `make install-local` installs for the current user and **never uses `sudo`**
  (it refuses a system prefix and refuses to run as root).
- `make install-system` runs the dedicated sudo installer and elevates **only**
  for the privileged copy into system locations.
- `make install` keeps the automatic behavior: a system prefix with an empty
  `DESTDIR` requests `sudo` for system paths only; a user-local prefix such as
  `PREFIX="$HOME/.local"` runs as the current user and does not elevate; staged
  `DESTDIR` installs never elevate.

Every variant first offers the base-code SHA-256 manifest step (yes/no);
`SLEELA_MANIFEST_CONSENT=yes|no|skip` answers it without a terminal. When a
system install elevates, `sudo` only copies new files into the system prefix as
the trusted system account — it never re-owns or alters files you already own,
and never touches your home directory. Avoid running `sudo make install` with a
user-home prefix; use `make install-local` as the target user instead.

The default executable directory is `$(PREFIX)/bin`, runtime support files are
under `$(PREFIX)/lib/sleela`, and shared documentation/examples are under
`$(PREFIX)/share/sleela`. The default configuration is
`/etc/sleela/startup.conf` for the default `/usr/local` prefix. For a custom
prefix, configuration defaults to `$HOME/.config/sleela/startup.conf`, so a
user-local install does not need root privileges. An existing configuration
is never overwritten. Use `BINDIR`, `LIBDIR`, `DATADIR`, and `SYSCONFDIR` to
override the corresponding destinations. On Linux, `/usr/local/bin` is preferred to
`/usr/bin` for a source installation so system package-manager files are not
overwritten. The uninstall target removes the three tracked executables (`sleela`, `sleelvac`, and `nordshrift`) and the tracked runtime/shared-data files for the selected prefix. It preserves configuration and refuses to remove untracked same-named binaries. Use the same `PREFIX`, `BINDIR`, `LIBDIR`, `DATADIR`, `SYSCONFDIR`, and `DESTDIR` values used during installation.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
