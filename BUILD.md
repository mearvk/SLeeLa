# SLeeLa Build and Platform Configuration

## Supported command-line targets

SLeeLa's native command-line runner is configured for:

- Linux
- Windows 10 and later
- macOS

The common runtime properties are stored in `config/sleela.properties.example`.

The source itself contains native platform backends in `impl/core` for:
- Windows: Win32 threads, Winsock, Win32 file I/O, LoadLibrary, ConPTY, and Windows timing.
- macOS: POSIX threads, sockets, filesystem APIs, `dlopen`, POSIX PTY, and Darwin timing.
- Linux: POSIX equivalents.

The front end uses `std::filesystem` and the platform backend rather than embedding
Linux-only process, path, terminal, or dynamic-library calls in the common C++
path.
The selected file is passed to native executables through the
`SLEELA_CONFIG_FILE` environment variable.

## Windows 10+

Use the MinGW-w64/GNU toolchain described by `build-windows.ps1`.

```powershell
powershell -ExecutionPolicy Bypass -File .\build-windows.ps1
```

The resulting executable is:

```text
impl\build\sleela.exe
```

Run a configured executable:

```powershell
.\impl\build\sleela.exe exec --config config\sleela.properties.example -- .\program.exe --number 5 --factor 0.95
```

The Windows build uses the repository's Win32 backend and GNU Make/MinGW
configuration. The documented minimum Windows command-line target is Windows
10.

## macOS

Use:

```bash
./scripts/build-macos.sh
```

The resulting executable is:

```text
impl/build/sleela
```

The build also stages the complete macOS runtime layout under `impl/build/SLeeLa/`, including `Config/sleela.conf` and `Options/OPTIONS.md`. The macOS-specific descriptive defaults are available at `config/macos.properties.example`; the actual Darwin/POSIX implementation remains in `impl/core`.

Run a configured executable:

```bash
./impl/build/sleela exec --config config/sleela.properties.example -- ./program --number 5 --factor 0.95
```

The macOS build uses clang/clang++ by default and the existing Unix/macOS
backend.

## Linux

Use:

```bash
./build-linux.sh
```

The common command-line interface is the same:

```bash
./impl/build/sleela exec --config config/sleela.properties.example -- ./program --number 5 --factor 0.95
```

## Common configuration values

```properties
ram.guard.enabled=true
ram.guard.sticks=1
ram.guard.policy=0.95
ram.guard.direction=decrease
ram.guard.write=false

platform.linux.enabled=true
platform.windows.enabled=true
platform.windows.minimum=10
platform.macos.enabled=true
platform.macos.minimum=12

sleela.native.config.environment=SLEELA_CONFIG_FILE
sleela.native.argument.separator=--
```

The RAM Guard values express a five-percent target reduction for one selected
memory stick. `ram.guard.write=false` deliberately keeps this at the
configuration/target level; SLeeLa does not claim a portable hardware write
interface for arbitrary RAM, firmware, timing, or voltage controls.

## Native executable arguments

Use `--` to separate SLeeLa options from the executable and its arguments:

```text
sleela exec --config FILE -- PROGRAM --flag 5 --factor 0.95
```

Arguments after the executable are passed through unchanged.

## Security gate

The existing SHA-256 execution gate remains part of the build/runtime design.
Before releasing a modified build, regenerate and verify the trusted manifest
on the trusted build host rather than treating a source-only change as a
verified binary release.


## Source-level platform verification

From the repository's `impl` directory, run:

```text
make test-platform
```

This exercises the native threading, networking, file-I/O, path, terminal/PTY,
dynamic-library, and time abstraction smoke tests for the detected platform.

On Windows 10+, the build selects the Win32 backend automatically when GNU Make
sets `OS=Windows_NT`. On macOS, Darwin is detected from `uname -s` and the
build omits the separate Linux `-ldl` requirement.

The macOS Defender provisioning path is intentionally not presented as a Linux
kernel-driver substitute. SLeeLa reports that no privileged Defender backend is
implemented there rather than attempting to load a Linux kernel module.


## HTTP 3.0 protocol build

The HTTP 3.0 protocol sources are built independently from the main SLeeLa runtime:

```sh
make -C http-3.0 port-test
make -C http-3.0 demo
make -C http-3.0 test
```

- `port-test` verifies the 160-bit logical PORT boundary and decimal/binary conversions.
- `demo` builds and runs the native HTTP 3.0 pipeline.
- `test` runs the C demo, Station tests, and Python HTTP 3.0 reference tests.

The HTTP 3.0 logical port namespace is protocol-level and must not be confused with the host OS's native TCP/UDP port range. Packet-field order is also a protocol parsing concern: authenticated any-order fields must be normalized to a canonical representation before MAC/DIGEST verification.
