# JetBrains Source Acquisition

This folder provides scripts for acquiring the official open-source JetBrains IntelliJ IDEA / IntelliJ Platform source tree.

Upstream: `https://github.com/JetBrains/intellij-community.git`

JetBrains identifies `intellij-community` as the open-source part of the JetBrains IDE codebase and the basis for IntelliJ Platform development.

Platform scripts: `download-source.sh`, `download-source-macos.sh`, `download-source.ps1`, and `download-source.cmd`.

The default acquisition is a shallow clone of the upstream `master` branch. Use the documented full-history option when complete Git history is required. Existing checkouts are not overwritten unless an explicit update option is supplied.

The scripts acquire upstream source only; they do not copy proprietary JetBrains components into SLeeLa or modify the upstream source tree.

**SLeeLa — MEARVK LLC — 2026**
