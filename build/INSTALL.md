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

Install Xcode Command Line Tools first, then build native products using their
macOS build scripts. For the SLeeLa runtime, use the root install Makefile only
after verifying the selected install target and its dependencies on the target
macOS version. Do not use Linux binaries on macOS.

## Windows 10+

Install Git and the MSYS2 UCRT64 MinGW-w64 toolchain for GNU Make-based native
targets. Build using `build/native-windows.ps1` from PowerShell. Install each
product using its documented Windows installer or deployment procedure; no
generic Windows installer is advertised until a Windows-native packaging target
is implemented and tested.

## Verification

Builds and installers should be tested on the destination OS. The native CI
workflow is the baseline for the runtime; product-specific dependencies,
optional connectors, GUI libraries, and platform packaging may need separate
installation steps.
