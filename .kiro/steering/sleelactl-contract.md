---
inclusion: fileMatch
fileMatchPattern: ['bin/sleelactl', 'bin/sleelactl.d/**', 'bin/SLEELACTL.md', 'tests/sleelactl_*_test.sh']
---

# `sleelactl` honesty and safety contract (registered 2026-11-11)

`sleelactl` promises it "never fakes an action a module cannot perform". These
rules were audited, fixed, and locked in by `tests/sleelactl_cli_test.sh`
(34 assertions fail against the pre-fix script). Keep them when editing.

1. **Registry verbs are honest.** Codec/compression/video Controllers are
   process-local. `list`/`probe`/`status` run the reference pipeline and report
   its real exit status. `load`/`unload`/`enable`/`disable`/`can` validate the
   id (non-negative integer, in `0..N-1` from the summary `(N handlers ...)`)
   and are then refused with exit 3. Never re-route them to the demo pipeline
   and print success.
2. **Failures are non-zero.** A failed run returns 1; `status all` is non-zero
   if any controllable module fails. Never end a failure branch in `warn`
   (which returns 0) without an explicit `return`.
3. **Diagnostics are kept.** Use `run_sleela`: it suppresses the runtime's
   stderr preamble on success and prints the last diagnostic lines on failure
   (`SLEELACTL_VERBOSE=1` shows everything). Do not add `2>/dev/null` to
   `sleela run`.
4. **Help and exit codes are documented and real.** `help <verb|module|nature>`
   works; unknown topics exit 2. Exit codes: 0 ok, 1 failed/absent, 2 usage,
   3 unavailable/not offered, 64 verb illegal for nature. Keep the help text,
   the script header, and `bin/SLEELACTL.md` in sync.
5. **Fan-out is safe.** Mutating daemon verbs with `all` require `--yes`;
   `--dry-run` prints the exact action and changes nothing; `all` skips
   process-local registry verbs.
6. **Bad input is rejected.** Unknown nature, unknown option, unknown module,
   and a `SLEELACTL_REGISTRY` that names a missing file all exit 2.
7. **No arbitrary choices.** `validate` prefers `validate` > `syntax` > `test`.
   Tool `run`/`inspect` resolve the CLI by name (`tool_binary_names`), never
   "first executable in a directory". Add a mapping there when registering a
   tool whose binary name differs from `<name>`/`sleela-<name>`.
8. **Tests gate it.** Run `sh tests/sleelactl_cli_test.sh` and
   `sh tests/sleelactl_systemctl_scope_test.sh` after any change;
   `.github/workflows/sleelactl.yml` runs them on Ubuntu (dash) and macOS.
   The script must stay POSIX sh: no `local`, no `[[`, no arrays, no
   process substitution; prefix function-local variables (`_rs_`, `_all_`, ...)
   because POSIX functions share globals. In tests, pass per-call environment
   with `env VAR=x cmd`, not `VAR=x function`.
