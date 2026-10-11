# SleelaTerminal Build and Platform Support

## Build the core shell

The default build requires a C++17 compiler and standard build tools:

```sh
make clean all
make test
./build/slsh -c 'echo hello'
```

The core build does not build or install the optional GTK desktop application.

## Optional GTK desktop application

On Linux, install GTK4 and VTE 2.91 GTK4 development packages, then run:

```sh
make all-gui
# or build just the GUI target
make gui
```

The GUI target is not advertised as portable to Windows or macOS until dependency and runtime behavior have been verified there.

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

For package staging, set `DESTDIR`, for example `make install DESTDIR=/tmp/package-root PREFIX=/usr`.

## Platform support and current limits

- **Linux:** primary development platform; CI runs the CLI build, smoke tests, sanitizers, and fuzz-target compilation.
- **macOS:** core CLI build and smoke tests are included in CI. Process groups, signals, FIFO lifecycle, and interactive terminal behavior still require platform-specific validation.
- **Windows:** native Windows support is not yet claimed. The implementation uses POSIX interfaces including `fork`/process execution, `termios`, `unistd`, signals, and FIFO-based process substitution. A Windows port needs an explicit OS abstraction layer or a POSIX compatibility environment and separate tests.

A successful compile is not proof of full POSIX conformance or production readiness. See `M5-HARDENING.md` and `PERFORMANCE.md` for outstanding validation gates.
