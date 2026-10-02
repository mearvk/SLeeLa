# JetBrains Source Download Method

## Upstream

`https://github.com/JetBrains/intellij-community.git`

The upstream project identifies this repository as the open-source part of the JetBrains IDE codebase and the basis for IntelliJ Platform development.

## Method

1. Use Git against the official repository.
2. Default to a shallow clone of `master`.
3. Support a full clone when history is needed.
4. Do not overwrite an existing checkout unless an update option is explicit.
5. Allow an explicit branch and destination.
6. Do not force-reset local changes during updates.
7. Keep acquisition separate from compilation.

## Platform scripts

- Linux / Unix: `download-source.sh`
- macOS: `download-source-macos.sh`
- Windows 10+: `download-source.ps1`

JetBrains documents additional module acquisition and build procedures upstream; those are intentionally separate from this source-acquisition layer.

**SLeeLa — MEARVK LLC — 2026**
