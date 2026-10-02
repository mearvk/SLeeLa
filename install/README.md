<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Quick and Safe Install

The planned installer is a three-platform deployment layer for Linux, macOS, and Windows 10+.

It should perform these stages in order:

1. Detect the operating system and repository root.
2. Check Git, the native compiler toolchain, and the SLeeLa build prerequisites.
3. Prompt for initial SLeeLa System settings.
4. Prompt for SLeeLa Server startup behavior.
5. Prompt for HTTP Server 1 through 9 startup behavior.
6. Prompt for VM Edition: Simple, Managed, or Advanced.
7. Prompt for Port Authority startup, pause, and shutdown behavior.
8. Build SLeeLa before deployment.
9. Stage the result before copying it.
10. Back up an existing user-local installation.
11. Copy the staged runtime and library files.
12. Write a user-local startup configuration.
13. Offer a user-level PATH update.
14. Report the installation and configuration paths.

The normal destination should be user-local so the installer does not need administrator or root access. It must not silently enable network listeners or modify system-wide service configuration.

## Platform entry points

- Linux: `quick-safe-install.sh`
- macOS: `quick-safe-install.sh`
- Windows 10+: `quick-safe-install.ps1`

The installer should keep build, copy, configuration, and PATH operations as separate stages so a failed build cannot produce a partial runtime installation.

The JetBrains source package remains separate. JetBrains documents the open-source `intellij-community` tree and its current Bazel-based build process upstream. The SLeeLa installer should not download or build JetBrains source unless that step is explicitly selected.

**SLeeLa — MEARVK LLC — 2026**