# SleelaTerminal Build and Platform Support

## Build the core shell

The default build uses a C++17 compiler and standard build tools:

```sh
make clean all
make test
./build/slsh -c 'echo hello'
```

The default `all` target builds the CLI and smoke-test executable. It does not require the optional GTK desktop dependencies.

## Optional GTK desktop application

On Linux, install GTK4 and VTE 2.91 GTK4 development packages, then run:

```sh
make all-gui
# or build just the GUI target
make gui
```

The GUI target is optional. Its dependencies and runtime behavior are not claimed to be portable to Windows or macOS without separate verification.

## Installation

Install the core CLI to the configured prefix (default `/usr/local`):

```sh
make install
```

Use a user-controlled prefix without administrator privileges:

```sh
make install PREFIX="$HOME/.local"
```

Install the optional GUI and its desktop entry separately:

```sh
make install-gui PREFIX="$HOME/.local"
```

For package staging, set `DESTDIR`, for example:

```sh
make install DESTDIR=/tmp/package-root PREFIX=/usr
```

## Platform support and current limits

- **Linux:** primary development platform; CI checks the CLI build and smoke tests, sanitizers, fuzz-target compilation, and the optional GTK/VTE GUI build.
- **macOS:** CI checks the core CLI build and smoke tests. Process groups, signals, FIFO lifecycle, and interactive terminal behavior still require platform-specific validation.
- **Windows:** native Windows support is not claimed. The implementation uses POSIX interfaces including `fork`/process execution, `termios`, `unistd`, signals, and FIFO-based process substitution. A Windows port needs an explicit OS abstraction layer or a POSIX compatibility environment and separate tests.

## Validation gates

The checked-in workflow defines the following automated gates:

| Gate | Platform | What it establishes |
|---|---|---|
| Clean CLI build + smoke tests | Ubuntu and macOS | Build and regression checks on the core shell |\n| Staged install/uninstall smoke test | Ubuntu and macOS | CLI installs under a temporary DESTDIR, runs, and uninstalls without touching host system paths |
| ASan/UBSan smoke run | Ubuntu | Sanitizer coverage for the smoke executable |
| Clang/libFuzzer build | Ubuntu | The fuzz target compiles; this is not a sustained fuzz campaign |
| Optional GUI build | Ubuntu | GTK4/VTE GUI compiles against the runner's packages |

A workflow definition is not evidence that the latest run passed. Check the repository's Actions page for the run associated with the commit under review.

## Release blockers and explicit non-goals

- Full POSIX shell conformance is not claimed. The lexer currently removes quote boundaries before expansion; therefore empty quoted arguments, quoted whitespace, glob suppression inside quotes, and `\"$@\"` positional-argument preservation are not yet guaranteed. Do not treat these as implemented until quote provenance is carried through expansion and regression tests pass. Special-builtin/function assignment semantics also need work.
- Interactive process groups, terminal job control, signal delivery, and child/FIFO cleanup need integration tests beyond the current smoke script.
- Windows-native support is not claimed. Do not advertise a native Windows build until a platform abstraction and Windows CI/test coverage exist.
- macOS CLI CI does not by itself establish full interactive-terminal or process-substitution parity.
- GUI compilation is not GUI runtime validation; window lifecycle, PTY behavior, keyboard input, and clipboard behavior need runtime testing on supported desktops.

A successful compile is not proof of full POSIX conformance or production readiness. See `M5-HARDENING.md` and `PERFORMANCE.md` for outstanding validation gates.
