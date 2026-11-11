---
inclusion: fileMatch
fileMatchPattern: ['bin/sleelactl', 'bin/sleelactl.d/**', 'bin/SLEELACTL.md', 'tests/sleelactl_*_test.sh']
---

# `sleelactl` design and honesty contract (2.0, registered 2026-11-11)

`sleelactl` promises it "never fakes an action a module cannot perform". These
rules are locked in by `tests/sleelactl_cli_test.sh`,
`tests/sleelactl_systemctl_scope_test.sh`, and
`bin/sleelactl check-registry --strict` (all in `.github/workflows/sleelactl.yml`).
Keep them when editing.

## Design rules (2.0)

1. **The registry is the single source of truth.** `bin/sleelactl.d/registry.conf`
   is format 2 (`format=2`; `name nature=... key=value ... summary="..."`). The
   script holds NO module-specific knowledge — no module names, unit names,
   binary names, pipeline filenames, or per-module branches. New behaviour is a
   new registry KEY handled generically per nature (add it to `KEY_TABLE`;
   `check-registry`, `entry_check`, and the docs table follow automatically).
   Legacy format-1 lines stay readable.
2. **Validate the registry, and enforce safety at use time.** `check-registry`
   rejects duplicates, unknown natures/keys, missing required keys, unsafe
   paths (absolute or `..`), malformed unit/label/service names, bad
   `handlers=`, and unknown/cyclic `after=`. `dispatch` calls `entry_check`
   in the CURRENT shell before acting (never rely on `usage_die` inside
   `$(...)` — it only exits the subshell). A module from `SLEELACTL_REGISTRY`
   is never escalated with `sudo` unless `SLEELACTL_TRUST_REGISTRY=1`.
3. **Registry verbs are only the honest ones:** `list` (declared handlers),
   `probe`/`status` (reference pipeline). `load/unload/enable/disable/can` are
   in-program Controller API: not CLI verbs (exit 64 with a pointer to
   `controller.<verb>(id)`). Do not reintroduce them unless a resident
   Controller host exists to hold the state.
4. **One key schema.** `KEY_TABLE` (nature|key|need|kind|description) drives
   `check-registry`, the use-time path checks (`entry_check`), and the docs
   table (`keys --markdown`, between `<!-- keys:begin/end -->`). Add or change a
   registry key there only; do not re-list keys in registry.conf's header.
5. **One nature table.** `NATURE_TABLE` in the script defines natures, verbs,
   and descriptions; `natures`, `help`, `completion`, and the docs table
   (`natures --markdown`, between `<!-- natures:begin/end -->` in
   `bin/SLEELACTL.md`) derive from it. The test fails if the docs drift.
6. **Daemons go through the service-manager adapter** (systemd `unit=`,
   launchd `launchd=`, Windows SCM `winsvc=`; `SLEELACTL_SERVICE_MANAGER`
   overrides auto-detection). No mapping for the active manager → exit 3, never
   a guess. Never invent service names or `after=` edges; register only what
   the real unit/plist/service definitions declare.
7. **Results on stdout, diagnostics on stderr** (`err`, `warn`, `hint` →
   stderr). `--json` is supported for `list`, `status`, `natures`, `keys`, `where`
   and rejected elsewhere; keep JSON valid (the test parses it).
8. **Parse the registry once** (`reg_require` → `REG`), not per lookup.

## Honesty and safety rules (carried from 1.1)

- Failures are non-zero; `status all` is non-zero if any controllable module
  fails. Never end a failure branch in `warn` without an explicit `return`.
- Use `run_sleela` (keeps diagnostics; `SLEELACTL_VERBOSE=1` for all). Never
  add `2>/dev/null` to `sleela run`.
- Exit codes: 0 ok, 1 failed/absent, 2 usage/unsafe/untrusted, 3 unavailable,
  64 verb illegal for nature. Keep help, script header, and docs in sync.
- Mutating daemon verbs with `all` require `--yes`; `--dry-run` prints the exact
  command (including `sudo`) and changes nothing; fan-out honours `after=`
  (dependencies first; reversed for stop/disable).
- `validate` prefers `validate` > `syntax` > `test`; tools run only the `bin=`
  CLI by name.

## Installer

`make -C install install-check` gates on `check-registry --strict`; `do-install`
writes completions to `<prefix>/share/sleela/completions/`; `sleelactl` is in
`install-runtime.py`'s `INSTALLED_BINARIES`, so uninstall removes it.

## Portability

POSIX sh and POSIX awk only (dash, bash --posix, mawk/BWK awk, `gawk --posix`):
no `local`, `[[`, arrays, process substitution, `gensub`, or `length(array)`.
Prefix function-scoped variables (`_rs_`, `_a_`, `_dm_`, ...) because POSIX
functions share globals. Use `set -f` around unquoted expansions of data. In
tests, pass per-call environment with `env VAR=x cmd`, not `VAR=x function`.
