<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Quick and Safe Install

The installer is a three-platform, user-local deployment layer for Linux, macOS, and Windows 10+.

## Installation flow

1. Detect the repository and platform.
2. Prompt for SLeeLa System startup.
3. Prompt for SLeeLa Server startup.
4. Prompt independently for HTTP Servers 1–9.
5. Prompt for VM Edition: `simple`, `managed`, or `advanced`.
6. Prompt for Port Authority startup, pause, and shutdown.
7. Prompt for user-level PATH configuration.
8. Build SLeeLa before deployment.
9. Stage the build before copying.
10. Back up an existing user-local installation.
11. Copy the staged `lib` and optional `bin` trees.
12. Write `config/startup.conf`.
13. Apply user-level `SLEELA_HOME` and `PATH` configuration (opt-in).
14. Report the installation, `SLEELA_HOME`, and configuration locations.

## Platform entry points

- Linux: `./install/quick-safe-install.sh`
- macOS: `./install/quick-safe-install-macos.sh`
- Windows 10+: `install\\quick-safe-install.cmd` or PowerShell directly.

Default destinations are:

- Linux/macOS: `$HOME/.local/sleela`
- Windows: `%LOCALAPPDATA%\\SLeeLa`

Set `SLEELA_INSTALL_ROOT` on Linux/macOS to choose another user-local destination.

## Safety model

The installer is deliberately user-local. It does not require sudo/root for its normal path, silently enable network listeners, install system-wide services, or alter system service configuration.

A build failure stops deployment before the staged result is copied. Existing installations are copied to a timestamped user-local backup before replacement.

PATH changes are user-level only.

- **Linux / macOS:** the installer writes an environment snippet at
  `$SLEELA_INSTALL_ROOT/profile/sleela-env.sh` that sets `SLEELA_HOME`, sets
  `SLEELA` to `$SLEELA_HOME/bin/sleela` (the toolchain executable), and prepends
  `$SLEELA_HOME/bin` to `PATH`, then adds one guarded `source` line to each shell
  startup file present (`~/.bashrc`, `~/.bash_profile`, `~/.zshrc`, `~/.profile`)
  so it applies to both bash (Linux default) and zsh (macOS default). The hook is
  idempotent — re-running the installer does not duplicate it — and `source`-ing
  the snippet makes `sleela` available in the current shell without reopening it.
- **Windows 10+:** the installer sets a user-level `SLEELA_HOME`, sets `SLEELA`
  to `%SLEELA_HOME%\bin\sleela.exe`, and prepends `%SLEELA_HOME%\bin` to the
  current user's `Path` (never the machine-wide PATH), and reflects all into the
  running session so new terminals inherit them automatically.

### The `sleela` executable

The toolchain binary (`sleela` / `sleela.exe`) is produced by the native `impl`
build (`make all`, which runs `make -C impl all`) and staged under
`impl/build/SLeeLa/bin`. The installer copies it into `$SLEELA_HOME/bin`, so a
successful install puts `sleela` on `PATH` **and** sets the `SLEELA` variable
that downstream projects look for (the "`set SLEELA=/path/to/sleela`"
convention). If the native build did not complete, the installer warns that
`sleela` is missing — run `make -C impl all` and re-install.

## Startup settings

The generated `startup.conf` records:

- SLeeLa System startup
- SLeeLa Server startup
- HTTP Server 1–9 startup
- VM Edition
- Port Authority startup
- Port Authority pause
- Port Authority shutdown

These settings describe startup policy; they do not by themselves grant network or operating-system authority.

## Commands

```text
make install-help
make install-check
./install/quick-safe-install.sh
./install/quick-safe-install-macos.sh
install\quick-safe-install.cmd
```

The JetBrains source acquisition helper remains separate and is not downloaded or built by this installer.

**SLeeLa — MEARVK LLC — 2026**