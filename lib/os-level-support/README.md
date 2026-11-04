# SLeeLa OS-level support library bindings

This package documents the SLeeLa-side contract for the portable native library in [`../../os-level-support/`](../../os-level-support/). The C/C++ API currently provides read-only process/host information and pure cron-expression calculations; the sibling `loader/` provides trusted shared-library loading.

## Boundary

These files are not an FFI binding. SLeeLa's VM does not currently link `osls_*` into its `os*` opcode bridge, so SLeeLa programs must continue to use the documented VM built-ins. Do not claim that `osls_get_system_info` or the cron helpers are callable from `.sleela` until a reviewed VM/native bridge and its tests are implemented.

The native API is intentionally user-mode and read-only for host facts and cron calculations. The native module loader executes loaded code in-process and is not a sandbox; only trusted modules should be loaded. See the [native API README](../../os-level-support/README.md) and [loader README](../../os-level-support/loader/README.md) for build and safety details.

## Planned integration seam

A future SLeeLa binding should define explicit VM built-ins/opcodes, argument and result validation, platform coverage, and tests before adding callable `.sleela` wrappers. Keep the C ABI in `os-level-support/osls.h` authoritative, and update the library inventory, native subsystem contract, integrity manifests, and cross-platform CI as part of that work.
