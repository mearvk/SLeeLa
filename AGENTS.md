# Agent instructions for SLeeLa

Entry point for AI coding agents and automated tools. Details live in the files linked below. Do not copy them here.

## Building an OS-specific binary set
Read `docs/BINARY-SETS.md` first, and use `docs/binary-sets.json` for module names, commands and the folders each module needs.

Hard rules:
- Publish executables only (no source folders, objects, libraries or fixtures), with a README row for every file and a `SHA256SUMS`.
- Do not start server binaries to describe them. Some open listeners immediately.
- Do not run installers or targets that write to `/usr` or `/etc`.
- Do not regenerate `security/sha256-manifest.json` just to pass the integrity gate. Follow section 6 of the guide.
- Only Linux x86-64 is verified. Label macOS and Windows sets as unverified unless you built and ran them.

## Other repository guidance
- Language and style rules: `.kiro/steering/` (for example `sleela-authoring-gotchas.md`, `comment-style.md`, `metadata-class-standard.md`).
- Build and test status: `.kiro/steering/build-status.md`.
- Platform support: `docs/PLATFORM-SUPPORT.md`.
