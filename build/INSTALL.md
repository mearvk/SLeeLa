# SLeeLa Installation Entry Points

## Linux

User-local installation (no sudo):

```sh
make install-local PREFIX="$HOME/.local"
```

System-wide installation:

```sh
sudo make install-system PREFIX=/usr/local
```

The installer may request the base-code SHA-256 manifest decision. For
automation, set `SLEELA_MANIFEST_CONSENT=yes`, `no`, or `skip` deliberately.

## macOS

Install Xcode Command Line Tools first. From the repository root, run
`./install/quick-safe-install-macos.sh`. The installer defaults to a
user-local runtime at `~/.local/sleela`, backs up an existing destination,
stages the library and integrity-manifest source files, installs native
executables, and can add the runtime's `bin` directory to `~/.zprofile`.
It builds through `build/native-macos.sh` unless `--no-build` is supplied.
Use `--system` for an administrator-approved install under `/usr/local`,
`--destination PATH` to choose a runtime root, and
`--skip-optional-products` if only `sleela` is built. The default requires
native executable files `impl/build/sleela`, `sleelvac`, and `nordshrift`.
Never install Linux binaries on macOS.

## Windows 10+

### GNU Make / MinGW-w64 runtime path

Install Git, PowerShell 5.1 or newer, Python 3, and the MSYS2 UCRT64 MinGW-w64
toolchain (including GNU Make and GCC/G++). From a PowerShell terminal at the
repository root, run:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
.\install\quick-safe-install.ps1
```

The installer defaults to a per-user destination under `%LOCALAPPDATA%\SLeeLa`
and does not require administrator rights. Choose system-wide installation only
from an elevated PowerShell. It stages files before changing the destination,
backs up an existing installation, installs the runtime binaries, writes
`config\startup.conf`, and can add the install's `bin` directory to PATH.
Use `-Destination 'D:\Apps\SLeeLa'` to specify a location. Use `-NoBuild`
when the Windows binaries have already been built; the default build path runs
`build/native-windows.ps1` rather than the Unix-oriented root Makefile.

The installer requires `impl\build\sleela.exe`, `sleelvac.exe`, and
`nordshrift.exe` by default. If a product is not yet available as a Windows
executable, `-SkipOptionalProducts` skips the compiler and Nordshrift while
still requiring the core `sleela.exe`. Windows packaging remains dependent on
building and testing each product natively; do not install Linux binaries on
Windows. The `.cmd` wrapper is also available at `install\quick-safe-install.cmd`.

### Visual Studio / MSVC development path

The initial MSVC project is for developers validating the existing platform
abstraction layer, not for installing the full SLeeLa runtime. With Visual
Studio 2022's Desktop development with C++ workload and CMake installed, run:

```powershell
.\build\windows\build-msvc.ps1 -Test
```

This builds the existing platform smoke tests with MSVC. It does not yet produce
`sleela.exe`, `sleelvac.exe`, or `nordshrift.exe`; use the MinGW-w64 runtime
path above for the current complete Windows build.

## Verification

Builds and installers should be tested on the destination OS. The native CI
workflow is the baseline for the runtime; product-specific dependencies,
optional connectors, GUI libraries, and platform packaging may need separate
installation steps.
