# JetBrains Source Acquisition

This folder provides platform-specific scripts for acquiring the official open-source JetBrains IntelliJ IDEA / IntelliJ Platform source tree.

Upstream: `https://github.com/JetBrains/intellij-community.git`

JetBrains identifies `intellij-community` as the open-source part of the JetBrains IDE codebase and the basis for IntelliJ Platform development.

## Platforms

- Linux / Unix: `download-source.sh`
- macOS: `download-source-macos.sh`
- Windows 10+: `download-source.ps1`
- Windows 10+ command launcher: `download-source.cmd`

The default is a shallow clone of the `master` branch. Use `--full` when complete Git history is required.

## Usage

```bash
bash jetbrains/download-source.sh
bash jetbrains/download-source-macos.sh
```

Windows PowerShell:

```powershell
powershell -ExecutionPolicy Bypass -File .\jetbrains\download-source.ps1
```

Windows command prompt:

```cmd
jetbrains\download-source.cmd
```

Supported acquisition options are `--destination PATH`, `--branch NAME`, `--full`, and `--update`. The PowerShell script exposes equivalent parameters.

The scripts acquire upstream source; they do not copy proprietary JetBrains IDE components into SLeeLa or modify the upstream checkout. Licensing and attribution remain governed by the upstream project.

JetBrains also documents `getPlugins.sh` / `getPlugins.bat` for additional modules. Those operations remain explicit upstream steps rather than being silently invoked here.

**SLeeLa — MEARVK LLC — 2026**
