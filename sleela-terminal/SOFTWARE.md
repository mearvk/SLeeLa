# CMD — SecureJDK 28 Software Center

The SleelaTerminal footer contains the **CMD** icon: a strict image control representing **CMD**, the Java native launcher associated with SecureJDK 28. The footer uses the image itself without a surrounding button outline or decorative frame. Clicking the CMD image opens the software center.

## Scan

**Scan GitHub Releases** queries the configured GitHub repository for published release metadata and distinguishes final releases from prereleases/alpha states. The scan is read-only.

## Installation / Preparation

For **CMD**, the current authoritative source is `mearvk/Ubuntu.Determinant.Beta.Restricted/tools/cmd` on the `main` branch. SleelaTerminal clones that repository, copies the `tools/cmd` source tree into `~/.local/share/sleela/software/cmd/`, and runs its `Makefile` when present. CMD is currently treated as a source-tree tool rather than a GitHub release asset. Other products continue to use the configured release/source mechanism under `~/.local/share/sleela/software/` and run a repository-provided `install.sh` or `scripts/install.sh` when one exists. If the source repository has no installer, the source is prepared locally without privileged system changes.

This is deliberately not a `curl | sh` mechanism. A future product manifest can attach SecureJDK 28, CMD, Asysma, or other products to their own release assets and verified build/install commands.

The canonical SecureJDK 28 design documentation is in `Ubuntu.Determinant.Beta.Restricted/markdown/SECUREJDK28.md`.

**SleelaTerminal™ — MEARVK LLC**


## CMD Source Reference

The current CMD implementation used by the SleelaTerminal installer is [`Ubuntu.Determinant.Beta.Restricted/tools/cmd`](https://github.com/mearvk/Ubuntu.Determinant.Beta.Restricted/tree/main/tools/cmd). It contains the CMD Makefile, `cmdlink`, `cmd-inspect`, `cmd-icon-gen`, and their C sources. Its README identifies CMD as the Java Command Executable Format targeting SecureJDK 28.
