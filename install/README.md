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
4. Stage the runtime, including the compiled `sleela` and `nordshrift` products.
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
