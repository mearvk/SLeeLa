---
inclusion: always
---

# Top-level `operating-system/` directory (renamed from `os/`)

The host-OS C/C++ driver subsystem that used to live at the repo-root `os/`
directory is now at **`operating-system/`**. Use the new path everywhere:
`operating-system/base_drivers.*`, `operating-system/include/sleela_os_*.h`,
`operating-system/src/sleela_os_*.c`, `operating-system/tests/test_os_*.c`,
`operating-system/Makefile`, and `operating-system/OS.md`.

- The old top-level `os/` path no longer exists — do not recreate it or refer
  to it in new code, docs, or paths.
- Path references were updated in `SHA256-DIGESTS.md` and
  `security/important-sha256-manifest.json`; the recorded SHA-256 digests match
  the moved files.
- **Unrelated:** the SLeeLa library package `lib/os/` (e.g. `SLLinuxOS`,
  `SLMacOS`, `SLWindowsOS`, `lib/os/OS.md`) is a different thing and was **not**
  renamed — keep using `lib/os/` as-is.
