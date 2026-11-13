# MEARVK.LLC — Max.Rupplin — `sleelactl` 2.1 Session Record

**Date:** 2026-11-11
**Repository:** `mearvk/SLeeLa`
**Branch:** `master`
**Subject:** Continued improvement of `sleelactl` after 2.0

Builds on ChatGPT's Start and today's earlier sessions (native audit, 1.1
quality pass, 2.0 design). Standing rules: `.kiro/steering/sleelactl-contract.md`.

## Evidence first

GitHub Actions confirmed what the sandbox could not test: the 2.0 commit passed
the `sleelactl` workflow on Ubuntu (dash, including the explicit dash pass) and
macOS (BWK awk), and Native Drive on Ubuntu, macOS, and Windows.

## Improvements

| Area | Change |
|---|---|
| Single key schema | `KEY_TABLE` (nature, key, need, kind, meaning) now drives `check-registry`, the use-time path checks, and `sleelactl keys [--markdown\|--json]`. `bin/SLEELACTL.md` embeds the generated table (drift-tested); `registry.conf` no longer re-lists keys. New validation: "needs one of" groups, comma-list kinds, launchd/Windows name checks. |
| Installer | `install-check` gates on `check-registry --strict`; `do-install` writes bash/zsh completions to `<prefix>/share/sleela/completions/`; `sleelactl` is recorded in the install manifest so uninstall removes it (pre-existing bug: it was installed but never tracked). |
| Installed layouts | `check-registry` summarises paths of modules not present in a partial install (`SLEELACTL_VERBOSE=1` lists them); `list <registry>` reports absent sources instead of "0 files". |
| Static analysis | New ShellCheck CI job. First (non-blocking) pass found 20 findings (5 warnings, 15 notes); all fixed or explicitly justified (validated argv/record splitting; literal `$` in generated completion code and test fakes); `ls` parsing replaced with globs. The job is now blocking at all severities. |

## Verification

- `tests/sleelactl_cli_test.sh` and `tests/sleelactl_systemctl_scope_test.sh`
  pass under sh, bash --posix, GNU awk, and gawk --posix.
- `make -C install install-check` passes (now includes `check-registry --strict`).
- Staged `DESTDIR` install: `sleelactl`, both completion files, and a manifest
  recording `sleelactl`; the installed copy resolves its registry (`where`),
  summarises absent modules, and `install-runtime.py uninstall` removes
  `sleelactl` and the completions.
- ShellCheck results come from CI (not available in the authoring sandbox).

*MEARVK LLC — 2026. Built on ChatGPT's Start.*
