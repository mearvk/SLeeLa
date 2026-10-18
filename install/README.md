<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Quick and Safe Install

The installer supports user-local and system-wide installation on Linux, macOS, and Windows 10+. User-local installation is the recommended default and does not require administrator access.

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
  `sudo`**. It defaults the prefix to `$HOME/.local`, refuses a system prefix
  (pointing you at `install-system`), and refuses to run as root.
- **`make install-system`** performs the system-wide install through
  [`system-install.sh`](system-install.sh). It runs the integrity step as the
  invoking (unprivileged) user first, then uses `sudo` **only** for the
  privileged copy into system locations.
- **`make install`** remains scope-aware: a user-local prefix stays as the
  current user, a system prefix (with no `DESTDIR`) elevates automatically for
  the system copy only.

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
