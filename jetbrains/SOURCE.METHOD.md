# JetBrains Source Download Method

## Objective

Provide a repeatable, platform-aware way to acquire the official open-source IntelliJ IDEA / IntelliJ Platform source tree.

## Acquisition policy

1. Use the official Git repository.
2. Default to a shallow clone.
3. Permit a full clone with `--full`.
4. Never overwrite an existing checkout unless `--update` is requested.
5. Permit explicit branch selection.
6. Default the checkout outside the SLeeLa source tree.
7. Do not execute downloaded source during acquisition.
8. Do not force-reset local changes during updates.

## Existing checkout

With `--update`, the scripts fetch the requested branch, check it out, and use `git merge --ff-only` against the remote branch. Local divergent changes are not discarded.

## Platform scripts

| Platform | Script |
|---|---|
| Linux / Unix | `download-source.sh` |
| macOS | `download-source-macos.sh` |
| Windows 10+ PowerShell | `download-source.ps1` |
| Windows 10+ command launcher | `download-source.cmd` |

## Build boundary

Source acquisition is separate from compilation. Follow the current JetBrains upstream build documentation for building the acquired source tree.

**SLeeLa — MEARVK LLC — 2026**
