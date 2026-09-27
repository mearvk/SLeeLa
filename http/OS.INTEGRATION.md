# SLeeLa HTTP OS Integration

The HTTP family uses a common OS abstraction and platform adapters.

Targets: Linux; Windows 10 and later; macOS. Windows future compatibility is capability/build based. As of September 2026, Microsoft documents Windows 11 releases including 24H2, 25H2 and 26H1. This project therefore does not invent a Windows 12 build number or claim an official Windows 12 release.

Rules: detect OS and architecture first; keep platform code under http/os; use native service managers only when explicitly requested; never open firewall ports merely because a package was installed; keep configuration outside binaries; uninstall only files owned by the package.

Linux service integration uses systemd where available. Windows service packages may use the Windows Service Control Manager. macOS daemon packages may use launchd.
