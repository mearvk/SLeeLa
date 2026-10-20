# SLeeLa Native C/C++ Portability

This guide covers native C and C++ source primarily outside `/lib`. It is a
repeatable baseline, not a claim that every directory has already been ported.
Each product's own Makefile or CMake project remains authoritative.

## Supported host targets

- Linux: GCC or Clang with the versions and libraries required by each product.
- Windows 10 and later: MSYS2 UCRT64 / MinGW-w64 for GNU Make-based targets.
  Visual Studio/CMake projects may use their own native Visual Studio generator.
- macOS: Xcode Command Line Tools (Apple Clang) and any product-specific SDKs.

## Build And Smoke-Test Entry Points

From the repository root:

```sh
# Linux
sh build/native-linux.sh

# macOS
sh build/native-macos.sh
```

Windows PowerShell (with MSYS2 MinGW-w64 tools available on PATH):

```powershell
powershell -ExecutionPolicy Bypass -File .\build\native-windows.ps1
```

These entry points currently build and run the native runtime platform/thread/
network smoke tests, Regex tests, and antivirus tests. Other native projects
have their own build requirements and must be added to this tested set as
their dependencies and platform assumptions are audited.

## Installation

The existing root installer remains the supported Linux installation path:

```sh
make install-local PREFIX="$HOME/.local"
```

It installs without sudo. System-wide installation is available through
`make install-system PREFIX=/usr/local` and may request elevated privileges.

For Windows and macOS, use the product-specific build scripts and install
instructions until a product has a native package/installer. Do not copy Linux
binaries across operating systems: build on the target OS or use the matching
CI runner. macOS signing/notarization and Windows MSI/MSIX packaging are release
tasks, not implied by a successful compile.

## Portability Rules For Native Code

- Use CMake or Makefile targets with explicit source lists; avoid compiling every
  source file in a directory indiscriminately.
- Prefer standard C/C++ APIs and isolate OS-specific calls behind platform
  adapters.
- Use `_WIN32`, `__APPLE__`, and platform feature checks rather than compiler
  name assumptions.
- Avoid POSIX-only headers, shell commands, path separators, and signals in
  shared code unless guarded by a platform abstraction.
- Check return values from filesystem, socket, thread, and process APIs.
- Keep generated files and binaries out of source directories; use ignored,
  platform-specific build output directories.
- Run the tests on the target OS. Cross-compilation alone does not establish
  runtime portability.

## CI

`.github/workflows/native-portability.yml` runs the native runtime build and
platform smoke tests on Linux, macOS, and Windows. Passing this workflow covers
the targets listed in that workflow only; it is not proof that all repository
subsystems compile on every platform.
