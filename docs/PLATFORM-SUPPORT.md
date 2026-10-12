# Platform Support and Validation Matrix

This matrix separates intended targets from evidence. A target is not fully supported merely because source code contains a backend.

| Area | Linux | macOS | Windows 10+ | Interpretation |
|---|---|---|---|---|
| Native C/C++ implementation | Build target | Build target | Build target | Require a fresh native build and tests on each release runner |
| Terminal / process support | CI exercised | CI exercised | Not established by current Terminal matrix | See [Terminal platform notes](../sleela-terminal/BUILD-PLATFORMS.md) |
| User Interface toolkit | X11 workflow | Cocoa workflow | Win32/MinGW workflow | Confirm latest run on each OS before release claims |
| Full repository test suite | CI configured | Not all layers necessarily run | Not all layers necessarily run | Known failures remain; do not describe the whole repository as green until resolved |

## Validation rules
1. Record OS, compiler/toolchain, build command, and test command for each release candidate.
2. Mark optional SDK-dependent modules as skipped when appropriate.
3. A workflow run with zero jobs is not a passing validation.
4. Report failed and pending runs accurately; do not infer success from old runs.
5. Require fresh green runs on the exact release commit for every claimed supported platform.

## Entry points
- Native build: `make -C impl all`
- Full suite: `bash ./test-suites/run-all.sh --all`
- UI toolkit: `make -C user-interface clean all`, then `make -C user-interface example` and `make -C user-interface smoke`
- Test policy: [`markdown/TESTING.md`](../markdown/TESTING.md)

This is a validation contract, not a claim that every component is production-ready.
