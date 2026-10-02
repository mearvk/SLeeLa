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
13. Apply only user-level PATH configuration.
14. Report the installation and configuration locations.

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

PATH changes are user-level only. On Linux/macOS the installer writes a small profile snippet; on Windows it updates the current user's PATH and never the machine-wide PATH.

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
