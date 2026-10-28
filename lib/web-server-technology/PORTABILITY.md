# slweb portability

The `slweb` library (C11 core + C++17 facade) is intentionally
**dependency-free**: no sockets, no HTTP wire handling, just the C and C++
standard libraries plus one tiny per-platform call for the CPU count. This
document is an honest per-target assessment.

## What is and is not platform-specific

- **Fully portable (no OS calls):** envelopment (programs, warrants, the
  Scientist-gated `active()` rule, longest-path-prefix `resolve()`), the rate
  limiter, constant-time compare, bearer-token parsing, client-IP resolution,
  and the keep-warm **band math**. These use only `<stdlib.h>`, `<string.h>`,
  `<ctype.h>`, `<stddef.h>` (C) and `<string>`, `<vector>`, `<optional>`,
  `<chrono>`, `<memory>` (C++).
- **One platform-specific call:** `slweb_warm_core_count()` reads the online CPU
  count — `GetSystemInfo` on Windows, `sysconf(_SC_NPROCESSORS_ONLN)` on
  POSIX/macOS. Everything else is identical across platforms.

## Target matrix

| Target | C core (`slweb.c`, C11) | C++ facade (`slweb.hpp`, C++17) |
|---|---|---|
| **Linux** (x86-64/arm64) | Yes — built and tested here | Yes — GCC/Clang |
| **macOS** (Intel + Apple Silicon) | Yes — `sysconf` is native | Yes — Apple Clang ≥ 10 (Xcode 10, 2018) |
| **Windows 11 / 10 / 8 / 7** | Yes — `GetSystemInfo` is on every version | Yes — MSVC 2017+ (`/std:c++17`) or MinGW-w64 GCC ≥ 7 |
| **Windows XP** | Yes, as **C11** with an old compiler | **No** — see below |

## macOS

Builds as-is with the shipped `Makefile`:

```sh
cd lib/web-server-technology
make        # clang produces build/libslweb.a and build/libslweb++.a
make test
```

Apple Clang has shipped C++17 since Xcode 10 (2018); `<optional>` and the rest
of the facade are available on every currently-supported macOS.

## Windows 7 through 11 (and newer)

Supported. The C and C++ paths both compile; the Windows branch of
`slweb_warm_core_count` uses `GetSystemInfo`, which exists on every Windows
version. Two toolchains work:

- **MSVC** (Visual Studio 2017 or later) with `/std:c++17`.
- **MinGW-w64** GCC ≥ 7 (the same toolchain `lib/sleela-sql` documents for its
  Windows build).

A `windows` Makefile target automates the MinGW-w64 cross-build (from Linux or
macOS) into `build/win/`:

```sh
make windows                       # x86-64 Windows libs (build/win/libslweb*.a)
make windows MINGW=i686-w64-mingw32 # 32-bit
make windows-test                  # also build the smoke-test .exe files
```

It auto-detects the toolchain and prints an install hint if MinGW-w64 is absent.
The equivalent manual commands:

```sh
x86_64-w64-mingw32-gcc  -std=c11   -O2 -c native/src/slweb.c   -Inative/include -o slweb.o
x86_64-w64-mingw32-g++  -std=c++17 -O2 -c cpp/src/slweb.cpp    -Inative/include -Icpp/include -o slweb_cpp.o
x86_64-w64-mingw32-ar   rcs libslweb.a  slweb.o
```

The Windows (`_WIN32`) code path has been **syntax/type-checked** on the build
host (forcing the branch with a minimal `windows.h` shim and `-fsyntax-only`,
clean under `-Wall -Wextra`). It has **not** been linked into a running `.exe`
in this environment — there is no MinGW cross-compiler or Windows host here, so
treat "builds on Windows" as verified at the compile level and pending a real
Windows/MinGW build for link + run confirmation.

## Windows XP (the one hard limit)

- The **C core is fine**: `slweb.c` is plain C11 (and uses only C89/C99-era
  constructs plus `GetSystemInfo`), so an old XP-era compiler can build it.
- The **C++17 facade is not** supportable on XP. No mainstream C++17 toolchain
  targets XP: MSVC's XP-targeting mode (`v141_xp`) is a C++14-era toolset and
  the C++17 standard library is not supported against the XP CRT. If XP is a
  hard requirement, consume the **C ABI** (`slweb.h`) directly and skip the C++
  facade.

## Note on the server vs. the library

This matrix is for the **library** only. The standalone server
(`web-server-technology/native/*`) uses POSIX sockets, `poll`, and threads and
is a POSIX/Linux/macOS artifact (its grades already report the Win32/Winsock
limitation at runtime). The library was kept socket-free precisely so it can be
embedded anywhere, including Windows, without that constraint.
