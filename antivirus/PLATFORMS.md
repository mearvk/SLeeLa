# Sleelavirin™ Native Engine — Platform & Build Matrix

**Author:** Max Rupplin — MEARVK LLC — 2026
**Companion to:** `NATIVE-ENGINE.md`

The native engine (`antivirus/src/`) is **portable ISO C11 + C++17** with no
third-party dependencies. All operating-system differences are isolated in one
small shim, `src/sleelavirin_compat.h` (formatted I/O and 64-bit file sizes).
This document lists the supported targets and how to build for each.

> It compiles and runs the same way on every target: SHA-256 hash matching,
> Aho–Corasick content matching, fuzzy similarity, Trap-2 heuristics, the C
> bridge, and the C++ wrapper. A `CLEAN` result is never proof of safety.

---

## Supported targets

| Target | Toolchain(s) | API floor | Status |
|---|---|---|---|
| **Windows 10+/11** | MSVC (VS 2017+) or MinGW-w64 | `0x0601` default | Supported |
| **Windows 7 / 8 / 8.1** | MSVC or MinGW-w64 | `0x0601` | Supported |
| **Windows XP / Server 2003** | XP-capable toolchain (see below) | `0x0501` | Supported (build with `WINXP`) |
| **macOS (Intel & Apple Silicon)** | Apple Clang / Xcode | `MACOSX_DEPLOYMENT_TARGET` | Supported |
| **Linux / Unix** | GCC or Clang | — | Supported (primary dev target) |

The engine only calls C-standard-library functions that have existed since the
Windows XP era (`fopen`, `fread`, `fseek`/`_fseeki64`, `malloc`, `snprintf` /
MSVC `_vsnprintf_s`), so a single source tree serves all three Windows
generations; only the API *floor* differs.

---

## Build options (pick one)

### 1. CMake — universal, recommended

```sh
cmake -S antivirus -B antivirus/build
cmake --build antivirus/build --config Release
ctest --test-dir antivirus/build --output-on-failure
```

- Selects MSVC on Windows automatically, or Ninja/MinGW if that is your
  generator; Apple Clang on macOS; GCC/Clang on Linux.
- Windows XP floor: add `-DSLEELAVIRIN_WINXP=ON`.
- Older macOS: add `-DCMAKE_OSX_DEPLOYMENT_TARGET=10.9` (or your minimum).

### 2. Windows PowerShell helper

```powershell
cd antivirus
./build-windows.ps1            # Win 7+ / 10+ (auto-detects CMake or MinGW-w64)
./build-windows.ps1 -WinXP     # Windows XP API floor
./build-windows.ps1 -Test      # build and run tests
```

### 3. MSVC `nmake` (no CMake)

From a *Developer Command Prompt for VS*:

```bat
cd antivirus
nmake /f Makefile.win          :: Win 7+ / 10+
nmake /f Makefile.win WINXP=1  :: Windows XP floor
nmake /f Makefile.win test     :: build and run tests
```

### 4. macOS shell helper

```sh
cd antivirus
./build-macos.sh               # Apple Clang
MACOSX_DEPLOYMENT_TARGET=10.9 ./build-macos.sh test
```

### 5. POSIX GNU Make (Linux/macOS/MinGW shells)

```sh
cd antivirus
make            # build C lib, C++ lib, tests, scanner
make test       # full test suite
CC=clang CXX=clang++ make
```

All paths produce the same artifacts: `libsleelavirin` (C), `libsleelavirin_cpp`
(C++), and `sleelavirin-scan` (CLI).

---

## Windows version floor, explained

`src/sleelavirin_compat.h` sets a Windows API floor only if the build has not
already chosen one:

- **Default = `_WIN32_WINNT = 0x0601` (Windows 7).** Binaries built this way run
  on Windows 7, 8, 8.1, 10, and 11.
- **`WINXP` / `-DSLEELAVIRIN_WINXP=ON` = `0x0501` (Windows XP / Server 2003).**

Building a binary that actually *loads* on Windows XP additionally requires a
toolchain that still emits XP-compatible executables, because modern linkers
default to a higher subsystem version. Options:

- **MSVC:** the `v141_xp` (or earlier `v140_xp`) platform toolset, i.e. Visual
  Studio 2017's XP-targeting toolset, with linker subsystem `5.01` (x86) /
  `5.02` (x64). Newer VS versions dropped the XP toolset.
- **MinGW-w64:** an older runtime/toolchain targeting the XP CRT.

The *source* already supports XP; the floor macro + an XP-capable toolchain are
all that is needed. If you only need Windows 7+, ignore `WINXP` entirely.

---

## macOS notes

- Builds with the stock Xcode Command Line Tools (`xcode-select --install`);
  no Homebrew packages are required.
- Universal/arch builds: pass `-DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"` to
  CMake (or `-arch x86_64 -arch arm64` in `CFLAGS`/`CXXFLAGS`).
- Set `CMAKE_OSX_DEPLOYMENT_TARGET` / `MACOSX_DEPLOYMENT_TARGET` to support older
  macOS releases; the code uses only base libc / libc++.

---

## Large files & 32-bit builds

`slv_file_size()` in the compat header uses the widest seek/tell each platform
offers (`_fseeki64`/`_ftelli64` on MSVC, `fseeko64` on MinGW, `fseeko` with
`_FILE_OFFSET_BITS=64` on POSIX), so file sizes above 2 GiB are handled where the
OS allows it. On a **32-bit XP build**, content scanning (which loads the whole
file into memory) refuses targets larger than `SIZE_MAX`; hash scanning streams
and is unaffected. Pair with the bridge's `max_file_size_bytes` to bound inputs.

---

## What is NOT platform-specific

The engine contains no OS security-API calls. Integrations such as Windows
**AMSI**, or a Linux/macOS system AV provider, are *external adapters* layered on
top of the portable engine (see `SCANNERS.md` and `platform/`), not part of this
C/C++ core. Keeping the core OS-neutral is what lets one source tree target XP
through Windows 11 and macOS without `#ifdef` sprawl.
