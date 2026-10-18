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
./install/quick-safe-install.sh
./install/quick-safe-install-macos.sh
install\\quick-safe-install.cmd
```

**SLeeLa — MEARVK LLC — 2026**

## Conventional `make install`

The conventional installation target is non-interactive and installs the built
native commands (`sleela`, `sleelvac`, and `nordshrift`) plus runtime libraries, verification tooling, documentation,
examples, and a default configuration. It does not replace the interactive
Quick and Safe installer above.

From the repository root:

```sh
make -C impl all
make install                              # auto-requests sudo for system paths
make install PREFIX="$HOME/.local"         # user-local; stays as current user
sudo make install                         # optional explicit system install
make install DESTDIR="$PWD/package-root"  # staged package installation
make install-check                        # verify installer entry points
make uninstall                            # remove tracked executables/runtime; preserve config
make uninstall PREFIX="$HOME/.local"       # remove a tracked user-local install
make -C install uninstall                  # lower-level equivalent
```

Privilege handling is scope-aware: when the destinations are system locations
and `DESTDIR` is empty, `make install` requests `sudo` automatically if needed.
For user-local destinations such as `PREFIX="$HOME/.local"`, installation runs
as the current user and does not elevate. Staged package installs using
`DESTDIR` do not request elevation. Avoid running `sudo make install` with a
user-home prefix; run it as the target user instead.

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
