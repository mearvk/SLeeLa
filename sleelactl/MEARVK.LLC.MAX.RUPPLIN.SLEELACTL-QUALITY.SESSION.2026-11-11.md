# MEARVK.LLC — Max.Rupplin — `sleelactl` Quality Session Record

**Date:** 2026-11-11
**Repository:** `mearvk/SLeeLa`
**Branch:** `master`
**Subject:** Second-pass quality audit of `bin/sleelactl` and repair of all eight findings

Builds on ChatGPT's Start (the original `sleelactl` control plane and nature
model) and the same-day native audit
(`MEARVK.LLC.MAX.RUPPLIN.SLEELACTL-NATIVE-AUDIT.SESSION.2026-11-11.md`).

## Registered as perfected

Each item below is fixed, covered by `tests/sleelactl_cli_test.sh`, gated in CI
(`.github/workflows/sleelactl.yml`), documented in `bin/SLEELACTL.md`, and
recorded as a standing rule in `.kiro/steering/sleelactl-contract.md`.

| # | Finding (before) | Resolution (after) | Status |
|---|---|---|---|
| 1 | `load/unload/enable/disable/can` ignored the verb and id and ran the same demo; `unload codecs 999` "succeeded" | `list/probe/status` run the reference pipeline honestly; mutations validate the id (integer, in `0..N-1`) then are refused with exit 3 because the Controller is process-local | Perfected |
| 2 | Pipeline failure exited 0; `status all` always 0 | Failures return 1; `status all` non-zero if any controllable module fails | Perfected |
| 3 | `sleela` stderr sent to `/dev/null` | `run_sleela` keeps diagnostics: preamble hidden on success, last lines shown on failure, `SLEELACTL_VERBOSE=1` for all | Perfected |
| 4 | `help <verb>` advertised but not implemented; exit codes undocumented | `help <verb|module|nature>`; unknown topic exits 2; exit codes 0/1/2/3/64 in help, header, and docs | Perfected |
| 5 | `stop/enable all` hit every daemon immediately | `--dry-run` shows exact actions; mutating daemon verbs with `all` require `--yes`; `all` skips process-local registry verbs | Perfected |
| 6 | Unknown nature and missing `SLEELACTL_REGISTRY` silently accepted | Both exit 2 with a clear message; unknown options and modules exit 2 | Perfected |
| 7 | `validate` took first Makefile target; tool `run` took first executable (ran `slecompiler-api-inspect` for `decompiler`; never found `debugger/sleela-debugger`); `$( : )` no-op | `validate` > `syntax` > `test`; tools resolve their CLI by name (`sleelac`, `sleela-decompiler`, `sleela-debugger`); no-op removed | Perfected |
| 8 | Only test was broken (registry override lacked `native-drive`) and ran nowhere | Scope test fixed; new functional suite (31 command checks, 20 output assertions); both run in CI on Ubuntu (dash) and macOS | Perfected |

## Verification

- `sh tests/sleelactl_cli_test.sh` — PASS (also under `bash --posix`).
- The same suite against the pre-fix `sleelactl` — 34 failures (proves it
  detects every regression above).
- `sh tests/sleelactl_systemctl_scope_test.sh` — PASS.
- Real repository: `probe codecs|compression|video`, `run math`,
  `info native-drive`, `available database`, `info http-9.0` all exit 0;
  `build native-drive <@native>.sleela` still builds the plugin; `status all`
  exits 1 here because the systemd units are not installed in the sandbox
  (the honest answer).
- dash was not available in the authoring sandbox; the CI workflow runs the
  suites under dash on Ubuntu.

## Found, not changed (outside sleelactl)

- `sleelactl validate http-1.0` fails because `http-1.0/http1_server.c`
  defines `http1_download_should_resume` twice (pre-existing; the previous
  `sleelactl` failed identically).

*MEARVK LLC — 2026. Built on ChatGPT's Start.*
