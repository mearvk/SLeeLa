# Platform Support and Validation Matrix

This matrix separates intended targets from evidence. A target is not fully supported merely because source code contains a backend.

| Area | Linux | macOS | Windows 10+ | Interpretation |
|---|---|---|---|---|
| Native C/C++ implementation | Build target | Build target | Build target | Require a fresh native build and tests on each release runner |
| Terminal / process support | CI exercised | CI exercised | Not established by current Terminal matrix | See [Terminal platform notes](../sleela-terminal/BUILD-PLATFORMS.md) |
| User Interface toolkit | X11 workflow | Cocoa workflow | Win32/MinGW workflow | Confirm latest run on each OS before release claims |
| Full repository test suite | CI configured | Not all layers necessarily run | Not all layers necessarily run | Known failures remain; do not describe the whole repository as green until resolved |

## Full repository build contract

The root `make all` target is the authoritative default build. It dispatches the
native runtime, XLT compiler, Java 28 bridge, regex/compiler/VM packages, installer checks,
tutorial validation, native smoke tests, configuration checks, CommonRails,
codecs, churn data checks, antivirus, registry, all eleven premium SLVM runners,
and Ballum. The optional database/data-source connector and API server targets
remain explicit because they have additional external dependencies.

The default build requires:

- GNU Make and a POSIX-compatible shell environment (`sh`, with Bash used by
  some repository scripts).
- A C11 compiler and a C++20-capable compiler.
- Python 3.
- A full JDK 28 (including JNI headers) for `java28`.
- OpenSSL development headers and libraries for the native network/security
  smoke test. The tests Makefile uses `pkg-config` when available and accepts
  explicit `OPENSSL_CFLAGS` / `OPENSSL_LIBS` overrides.

### Host prerequisites

| Host | Recommended toolchain | Additional development packages |
|---|---|---|
| Linux | GCC/G++ or Clang, GNU Make | OpenSSL development package, X11, FreeType, Fontconfig, pkg-config |
| macOS | Xcode Command Line Tools, Apple Clang, GNU Make | Homebrew OpenSSL 3 and pkg-config; Apple frameworks come from the SDK |
| Windows 10+ | MSYS2 UCRT64 MinGW-w64 GCC/G++, GNU Make, Bash | OpenSSL and pkgconf from the matching UCRT64 repository; JDK 28 with JNI headers |

Windows builds are supported through MinGW-w64, not MSVC. The current native
runtime compatibility layer uses GCC-specific facilities (including
`#include_next`). Run the root build from an MSYS2 UCRT64 shell so the POSIX
build utilities used by the Makefiles are available. Winsock is linked through
`ws2_32`; native executables use the `.exe` suffix.

Example Windows MSYS2 package installation:

```sh
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-openssl mingw-w64-ucrt-x86_64-pkgconf
```

### Build and validation sequence

After intentional source changes, regenerate the trusted manifest from the
repository root before running the fail-closed build gate:

```sh
make -C impl regen-manifest
make all
```

The `make all` target invokes the repository's default tests and smoke checks;
it is not a substitute for a fresh host-specific release qualification. Record
the compiler/JDK versions and the actual command output for each OS. Do not mark
a platform green based only on source-level portability changes or a successful
dry run.

The platform helper scripts have narrower scopes: `scripts/build-linux.sh`,
`scripts/build-macos.sh`, and `build-windows.ps1` build the core runtime and
XLT compiler. They do not replace the root `make all` repository build.

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
