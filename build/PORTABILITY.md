# SLeeLa Native C/C++ Portability

This guide covers native C and C++ source primarily outside `/lib`. It is a
repeatable baseline, not a claim that every directory has already been ported.
Each product's own Makefile or CMake project remains authoritative.

## Supported Host Targets

- Linux: GCC or Clang with the versions and libraries required by each product.
- Windows 10 and later: MSYS2 UCRT64 / MinGW-w64 for GNU Make-based targets.
  An initial Visual Studio/MSVC CMake target is also available for the existing
  OS-abstraction smoke tests; it is not yet a full MSVC runtime build.
- macOS: Xcode Command Line Tools (Apple Clang) and any product-specific SDKs.

## Build And Smoke-Test Entry Points

From the repository root:

```sh
# Linux
sh build/native-linux.sh

# macOS
sh build/native-macos.sh
```

Windows PowerShell with MSYS2 MinGW-w64 tools available on PATH:

```powershell
powershell -ExecutionPolicy Bypass -File .\build\native-windows.ps1
```

Windows PowerShell with Visual Studio 2022 and its Desktop development with C++
workload installed:

```powershell
powershell -ExecutionPolicy Bypass -File .\build\windows\build-msvc.ps1 -Test
```

The MSVC project compiles the existing files under `impl/core/` in place and
writes all generated files to `build/msvc-platform/`. It tests the thread,
file I/O, network, path, terminal, native-library-loader, and time abstraction
layers. It does not relocate or rewrite source files, and it does not yet build
the complete `sleela`, `sleelvac`, or Nordshrift products with MSVC.

The MinGW entry point currently builds and runs the native runtime platform,
thread, and network smoke tests, Regex tests, and antivirus tests. Other native
projects have their own build requirements and must be added to the tested set
as their dependencies and platform assumptions are audited.

## Source-Preservation Rule

Keep existing C and C++ source files in their current directories. Add
platform-specific build definitions and scripts alongside the existing build
system. Do not copy shared source into side folders or rewrite it merely to
accommodate a second compiler. If MSVC reveals a genuine portability issue,
record the file and compiler diagnostic and make only a minimal, reviewed fix.

Keep build artifacts isolated by toolchain: the existing Make-based outputs
remain under their current build directories, and the MSVC smoke suite uses
`build/msvc-platform/`.

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

`.github/workflows/native-portability.yml` runs the current native runtime
build and platform smoke tests on Linux, macOS, and Windows MinGW-w64, plus a
separate MSVC job for the initial OS-abstraction slice. Passing these jobs covers
only the targets listed above; it is not proof that all repository subsystems
compile on every platform.
