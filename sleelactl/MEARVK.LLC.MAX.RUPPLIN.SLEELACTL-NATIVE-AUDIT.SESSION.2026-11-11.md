# MEARVK.LLC — Max.Rupplin — `sleelactl` / `@native` Audit and Repair Session Record

**Date:** 2026-11-11
**Repository:** `mearvk/SLeeLa`
**Branch:** `master`
**Subject:** Auditing the `sleelactl` + `@native` promise against structure, organization, components, connectedness, correctness, and quality, then closing the gaps found

## Credit

This work stands on **ChatGPT's Start**. The `sleelactl` control plane, the
Native Drive tool, the `@native` per-method AOT emitter, artifact-v4 method
metadata, explicit host binding, and the native regression fixtures that this
session audited and repaired all come from her groundwork. This session (Kiro)
connected those pieces end to end and made their tests run and gate. Thank you,
ChatGPT.

## 1. Audit findings (before)

| Axis | Finding |
|---|---|
| Structure | Sound: nature-classified verbs, one-line registry, all 38 registry handles resolve |
| Components | Emitter, IPC handle table, concurrency lifecycle each worked in isolation |
| Correctness | 3 of 5 native targets failed on `master` (`test-native-call`, `test-native-method-metadata`, `test-native-method-aot`) |
| Connectedness | `sleelactl build native-drive x.sleela` could not produce a plugin; the driver could not find a fresh `impl/build/sleela` |
| Quality | Docs said the emitter was on a branch / whole-program; native targets were outside `make test` and CI |

Root causes:

1. The semantic checker did not know `nativeOpen` / `nativeCall` / `nativeClose`
   (only the compiler lowered them), so `check`/`run` rejected documented
   syntax-3.0 code.
2. The native fixtures used `static int main()`, which the checker rejects
   ("main must return void"). `native_drive_compile.sleela` had also lost its
   `fallback` bytecode method, so the expected result 105 was unreachable.
3. `runCommand()` in `impl/frontend/driver.cpp` echoed the security-verification
   command to **stdout**, polluting `sleela aot --emit` C output.
4. `tools/sleela-native-drive.py` still assumed whole-program emission with a
   `main`, refused `--kind shared` for `.sleela`, did not pass the loader
   include path, and only looked for `sleela` on `$SLEELA`/PATH.

## 2. Changes

| Path | Change |
|---|---|
| `impl/frontend/semantic.cpp` | `nativeOpen/nativeCall/nativeClose` are checked built-ins (3.0 gate, arity, String path/method) |
| `impl/core/sleela_core.{h,c}` | New `slvm_find_func()` and `slvm_load_file_with_entry()` so embedders/tests can select a non-`main` entry |
| `impl/tests/native_drive/*.sleela` | `void main()` + `int check()`; restored `fallback`; not-handled fixture initialises its counter in `check()` (static initialisers run in `main`'s prologue) |
| `impl/tests/native_drive/*_smoke.c`, `impl/Makefile` (generated smoke) | Harnesses load with `slvm_load_file_with_entry(path, "Class::check")` |
| `impl/frontend/driver.cpp` | `[defender] $ ...` echo goes to stderr |
| `impl/nordshrift/native_method_emit.cpp` | Fixed `-Wmisleading-indentation` |
| `tools/sleela-native-drive.py` | `.sleela` builds a typed-ABI plugin (`--kind shared` default), `-DSL_NATIVE_PLUGIN_BUILD -I<native_loader.h dir>`, `--native-include`, sleela discovery via `--sleela`/`SLEELA`/`SLEELA_BIN`/PATH/repo build |
| `bin/sleelactl` | Exports its resolved `SLEELA_BIN` as `SLEELA` to the driver |
| `install/Makefile` | Installs `native_loader.h` to `<runtime>/include/` |
| `impl/Makefile` | Native targets added to `make test` |
| `.github/workflows/native-drive.yml` | Linux builds `sleela`, runs call/metadata/aot/concurrency targets and a `sleelactl` end-to-end `@native` plugin build; broader path triggers |
| `impl/native-drive/{README,AT-NATIVE-DISPATCH}.md`, `pi-generator/AT-NATIVE.md`, `bin/SLEELACTL.md` | Stale branch / whole-program / "no CI" claims corrected |
| `security/sha256-manifest.json` | Regenerated (13,739 entries; `check-manifest` OK) |

## 3. Verification (Linux, this session)

- `make -C impl build/sleela` — OK
- `make -C impl test` — exit 0, including all five native targets:
  `test-native-ipc-handles`, `test-native-call`, `test-native-method-metadata`,
  `test-native-method-aot` (generated plugin + NOT_HANDLED + boundary +
  semantic-reject + library-unit), `test-native-concurrency`
- `tools/tests/test_native_drive.py` (20 tests) and `test_sleela_ipc_worker.py` — OK
- `sleelactl build native-drive impl/tests/native_drive/native_method_aot_compile.sleela --require-sleela-abi -o aot.so`
  — builds; exports `sl_native_abi_version`, `sl_native_typed_abi_version`,
  `sl_native_run`, `sl_native_call`; ABI versions load as 1/1
- `sleelactl build native-drive x.sleela --kind executable` — refused with a clear message
- `sleelactl inspect native-drive xclass ...` — now reaches the real loader

## 4. Still open

- macOS/Windows execution of the VM native targets (CI runs them on Linux only).
- `make test-native-*` race stress and sanitizer coverage.
- Pre-existing, unrelated: `make test-exceptions` fails to link
  (`build/exception_vm_smoke` omits `-fopenmp` for `libslcore.a`); it is not in
  the `make test` aggregate.
- `sleelactl <verb> all` for mutating daemon verbs has no dry-run/confirmation.

*MEARVK LLC — 2026. Built on ChatGPT's Start.*
