# MEARVK.LLC — Max.Rupplin — `sleelactl` 2.0 Design Session Record

**Date:** 2026-11-11
**Repository:** `mearvk/SLeeLa`
**Branch:** `master`
**Subject:** Design review of `sleelactl` and implementation of all twelve improvements

Builds on ChatGPT's Start (the original `sleelactl` control plane and nature
model) and the two earlier sessions today (native audit; 1.1 quality pass).
The standing rules are registered in `.kiro/steering/sleelactl-contract.md`.

## High value

| # | Improvement | What changed |
|---|---|---|
| 1 | Registry is the single source of truth | Registry format 2 (`format=2`, keyed `key=value`, quoted summaries). All module knowledge moved out of the script: tool binaries (`bin=`), registry pipelines (`demo=`), handler counts (`handlers=`), native-drive's driver/helpers (`script=`, `runsub=`, `sub.service=`, `doc=`), simulators (`sim=`), service names per manager. The dispatcher has no per-module branch. Legacy format-1 lines are still read. |
| 2 | Registry validation and privilege safety | `sleelactl check-registry [--strict]` (duplicates, unknown natures/keys, required keys, unsafe paths, malformed unit/label/service names, bad `handlers=`, unknown/cyclic `after=`, missing source paths). The same rules are enforced at use time in the current shell (`entry_check`), and a module from `SLEELACTL_REGISTRY` is never escalated with `sudo` unless `SLEELACTL_TRUST_REGISTRY=1`. CI runs `check-registry --strict`. |
| 3 | Stop advertising impossible verbs (option a) | Registry nature verbs are now `list probe status`. `load/unload/enable/disable/can` are documented as in-program Controller API; the CLI returns 64 with a pointer to `controller.<verb>(id)`. `list <registry>` reports declared handlers without running anything (it was previously unreachable, shadowed by the cross-cutting `list`). A resident Controller host remains a possible future option (b). |

## Medium and lower value

| # | Improvement | What changed |
|---|---|---|
| 4 | Machine-readable output | `--json` for `list`, `status`, `natures`, `where` (rejected elsewhere). |
| 5 | One nature table | `NATURE_TABLE` drives `natures`, `help`, `completion`, and the docs table (`natures --markdown`, embedded in `bin/SLEELACTL.md` between markers; the test fails on drift). |
| 6 | Service-manager adapter | systemd (`unit=`), launchd (`launchd=`; real labels `com.mearvk.sleela-server[3]` from the shipped plists), Windows SCM (`winsvc=`; none registered — the repo defines no Windows services). `SLEELACTL_SERVICE_MANAGER` override; no mapping → exit 3. |
| 7 | Output streams | `warn`, `err`, and new `hint` go to stderr; results to stdout. |
| 8 | `where` / `doctor` | Resolved root, registry and its source (beside/installed/override), format, module count, runtime + version, Python, service manager, scope; also `--json`. |
| 9 | Subject coverage | Registered `physics`, `chemistry`, `astrophysics`, `sociology`, `economics`, `inference`, `financial` alongside `math` — each demo verified to run with the built `sleela`. |
| 10 | Dependency-ordered fan-out | `after=` edges; `start/restart/enable all` bring dependencies up first, `stop/disable all` reverse. No edges are registered because the real unit files declare none between SLeeLa daemons. |
| 11 | Parse once | The registry is parsed once into memory; `list` went from 0.23 s to 0.02 s. |
| 12 | Shell completion | `sleelactl completion bash|zsh`, generated from the registry (verbs, natures, modules per verb). |

## Bugs found and fixed while doing it

- `usage_die` inside `$(...)` only exited a subshell, so an unsafe-path refusal
  did not stop the command; validation now runs in the current shell.
- The registry `list` verb could never be reached (the cross-cutting `list`
  command consumed it).
- Installed layouts resolve registry paths under `<prefix>/lib/sleela/` again
  (the 1.x `runtime_file` behaviour), verified with a simulated install tree.

## Verification

- `tests/sleelactl_cli_test.sh` — 53 command checks plus 30 output/ordering
  assertions — PASS under `sh`, `bash --posix`, GNU awk, and `gawk --posix`.
- `tests/sleelactl_systemctl_scope_test.sh` — PASS (adds untrusted-override
  sudo refusal; keeps a legacy format-1 registry for compatibility).
- `bin/sleelactl check-registry --strict` on the real registry — 43 modules,
  0 errors, 0 warnings.
- Real repository: all three registry probes, all eight subjects (spot-checked
  math/physics/financial plus the rest earlier), native-drive build of a
  `@native` plugin, protocol validate, datasource availability — exit 0.
- dash, mawk, and BWK awk were not available in the authoring sandbox; the CI
  workflow runs the suites under dash on Ubuntu and BWK awk on macOS.

## Behaviour changes to note

- `sleelactl load|unload|enable|disable|can <registry> ...` now exit 64 (was 3).
- `sleelactl status all` / daemon verbs on macOS now use launchd instead of
  failing for lack of `systemctl`.
- A `SLEELACTL_REGISTRY` override can no longer drive `sudo` without
  `SLEELACTL_TRUST_REGISTRY=1`.

*MEARVK LLC — 2026. Built on ChatGPT's Start.*
