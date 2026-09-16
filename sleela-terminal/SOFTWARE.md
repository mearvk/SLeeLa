# SecureJDK 28 / CMD Software Center

The SleelaTerminal footer contains a compact Java/SecureJDK-style action icon based on the supplied visual reference. Clicking it opens a software center.

## Scan

**Scan GitHub Releases** queries the configured GitHub repository for published release metadata and distinguishes final releases from prereleases/alpha states. The scan is read-only.

## Installation / Preparation

The first implementation refreshes or clones source under `~/.local/share/sleela/software/` and runs a repository-provided `install.sh` or `scripts/install.sh` when one exists. If the source repository has no installer, the source is prepared locally without privileged system changes.

This is deliberately not a `curl | sh` mechanism. A future product manifest can attach SecureJDK 28, CMD, Asysma, or other products to their own release assets and verified build/install commands.

The canonical SecureJDK 28 design documentation is in `Ubuntu.Determinant.Beta.Restricted/markdown/SECUREJDK28.md`.

**SleelaTerminal™ — MEARVK LLC**
