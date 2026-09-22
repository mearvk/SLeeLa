# SLeeLa Build and Platform Configuration

## Supported command-line targets

SLeeLa's native command-line runner is configured for:

- Linux
- Windows 10 and later
- macOS

The common runtime properties are stored in `config/sleela.properties.example`.
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
