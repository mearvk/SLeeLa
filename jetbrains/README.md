<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# JetBrains Source Acquisition

This folder provides platform-specific scripts for acquiring the official open-source JetBrains IntelliJ IDEA / IntelliJ Platform source tree.

Upstream: `https://github.com/JetBrains/intellij-community.git`

JetBrains identifies `intellij-community` as the open-source part of the JetBrains IDE codebase and the basis for IntelliJ Platform development.

## Platforms

- Linux / Unix: `download-source.sh`
- macOS: `download-source-macos.sh`
- Windows 10+: `download-source.ps1`

The default acquisition is a shallow clone of the upstream `master` branch. Use the full-history option when complete Git history is required.

## Usage

```bash
bash jetbrains/download-source.sh
bash jetbrains/download-source-macos.sh
```

Windows PowerShell:

```powershell
powershell -NoLogo -NoProfile -File .\jetbrains\download-source.ps1
```

The scripts support destination, branch, full-history, and explicit-update controls. Existing checkouts are not overwritten unless an update option is supplied.

The scripts acquire upstream source only; they do not copy proprietary JetBrains components into SLeeLa or modify the upstream source tree.

JetBrains also documents additional module acquisition and build procedures upstream; those remain explicit upstream steps.

**SLeeLa — MEARVK LLC — 2026**