<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# Build Scripts for SLeeLa

This directory contains build and installation scripts for SLeeLa.

## Available Scripts

### `build-linux.sh` — Linux Build & Install
Builds SLeeLa for Linux (gcc/g++) and installs to `/bin` or a custom directory.
This builds three binaries: `sleela`, `nordshrift`, and the XLT CPU-model
compiler `xltc` (from `impl/xlt`). When `BIN_DIR` is set, all three are installed.

**Usage:**
```bash
# Install to /bin (requires sudo)
sudo ./build-linux.sh

# Install to user-local directory
BIN_DIR=$HOME/bin ./build-linux.sh

# Use custom compilers
CC=clang CXX=clang++ ./build-linux.sh
```

**Environment variables:**
- `CC` — C compiler (default: `gcc`)
- `CXX` — C++ compiler (default: `g++`)
- `BIN_DIR` — Installation directory (default: `/bin`)
- `CFLAGS` — Custom C compiler flags
- `CXXFLAGS` — Custom C++ compiler flags

**Requirements:**
- C11-compatible C compiler (gcc or clang)
- C++17-compatible C++ compiler
- `make`
- pthread development libraries

---

### `build-macos.sh` — macOS Build & Install
Builds SLeeLa for macOS (clang/clang++) and installs to `/usr/local/bin` or custom directory.
Like the Linux script, this builds and installs `sleela`, `nordshrift`, and the
XLT compiler `xltc`.

**Usage:**
```bash
# Install to /usr/local/bin (requires sudo)
sudo ./build-macos.sh

# Install to user-local directory
BIN_DIR=$HOME/bin ./build-macos.sh
```

**Requirements:**
- Xcode Command Line Tools: `xcode-select --install`
- C11-compatible C compiler (clang)
- C++17-compatible C++ compiler
- `make`

---

### `clean-build.sh` — Full Clean Rebuild
Performs a complete clean rebuild from scratch, with optional test suite execution.

**Usage:**
```bash
# Full rebuild with tests
./clean-build.sh

# Rebuild without running tests
./clean-build.sh --no-test
```

**Environment variables:**
- `CC` — C compiler (default: `gcc`)
- `CXX` — C++ compiler (default: `g++`)

In addition to the full clean rebuild, this script clean-builds the XLT compiler
(`xltc`) and runs an XLT smoke test in both directions: a congruent model
(`epyc-ddr2.xlt`) must compile, and a deliberately bad model (`bad-model.xlt`)
must be rejected.

---

### `xlt-check.sh` — XLT & GUI Inventory Check
Fail-closed verifier for the XLT front-end and the GUI version lines. Verifies
the `impl/xlt` sources and Makefile, the `lib/xlt` README and `.xlt` examples,
that `xltc` (when built) accepts a congruent model and rejects a bad one, and
that each `lib/gui/version-*` line ships an `SLPackage.sleela` manifest, a
`gui-demo.sleela`, and — where present — a valid `preview/` directory.

**Usage:**
```bash
./scripts/xlt-check.sh
```
Building `xltc` is not required to run the check; the live-compile step is
skipped (not failed) when `impl/xlt/build/xltc` is absent.

---

### `autocheck-toolchain.sh` — Toolchain Autocheck
Verifies core toolchain assets (verifier tool, SHA-256 manifest, `impl/Makefile`,
the XLT Makefile and examples) and that every GUI version line carries its
`SLPackage.sleela` manifest, then scans build files for stale path assumptions.

### `tutorial-check.sh` — Tutorial & Examples Check
Verifies the `tutorial/` lesson inventory and the `examples/*.xml` projects
(well-formedness and expected-evidence pairing).

### `validate-config.py` — Configuration Validator
Validates the SLeeLa configuration policy (`config/*.conf`) and the
`*.properties.example` templates without modifying them.

---

## XLT CPU-model compiler (`xltc`)

The XLT front-end lives in `impl/xlt` (lexer, parser, resolver, and the `xltc`
CLI) and compiles `.xlt` CPU-model sheets found in `lib/xlt/examples`. It is
self-contained — its own `Makefile`, no dependency on the Nordshrift build — and
produces `impl/xlt/build/xltc`.

```bash
# Build just the XLT compiler
make -C impl/xlt

# Compile a model sheet
./impl/xlt/build/xltc lib/xlt/examples/epyc-ddr2.xlt

# Built-in smoke check (compiles the bundled EPYC+DDR2 model)
make -C impl/xlt check
```

The build scripts above build and install `xltc` alongside `sleela` and
`nordshrift`, so after a `BIN_DIR` install you can run `xltc` directly.

---

## Quick Start

### Linux (Ubuntu/Debian)

```bash
# Install build tools
sudo apt-get install build-essential

# Clone and build
git clone https://github.com/mearvk/SLeeLa.git
cd SLeeLa
sudo ./scripts/build-linux.sh
```

### macOS

```bash
# Install Xcode Command Line Tools (if not already installed)
xcode-select --install

# Clone and build
git clone https://github.com/mearvk/SLeeLa.git
cd SLeeLa
sudo ./scripts/build-macos.sh
```

### User-Local Installation (No sudo)

```bash
# Create a user bin directory if it doesn't exist
mkdir -p $HOME/bin
export PATH="$HOME/bin:$PATH"  # Add to ~/.bashrc or ~/.zshrc to persist

# Run build script
cd SLeeLa
./scripts/build-linux.sh  # or build-macos.sh
```

---

## Verification

After installation, verify the build:

```bash
sleela version
nordshrift version
xltc lib/xlt/examples/epyc-ddr2.xlt   # compiles a sample CPU-model sheet
```

Run example programs:

```bash
SLEELA_SHEET=./SHEET.sheet sleela run impl/examples/hello.sleela
```

---

## Troubleshooting

### Compiler not found
Ensure you have a C11 and C++17 compatible compiler installed:
- **Linux:** `sudo apt-get install build-essential`
- **macOS:** `xcode-select --install`

### Permission denied
If you don't have sudo access or want to avoid it:
```bash
BIN_DIR=$HOME/bin ./scripts/build-linux.sh
```

### Build failures
Run the clean build script to ensure a fresh start:
```bash
./scripts/clean-build.sh
```

Check the build output for compiler errors. Make sure your compiler versions support C11 and C++17:
```bash
gcc --version  # Should be 5.0 or later
g++ --version  # Should be 5.0 or later
```

---

## Manual Build (Without Scripts)

If you prefer to build manually:

```bash
cd impl
make              # Build sleela and nordshrift binaries
make test         # Run test suite
make clean        # Remove all build artifacts

# Binaries appear in build/
ls -lh build/{sleela,nordshrift}

# Build the XLT compiler separately (self-contained)
make -C xlt       # produces impl/xlt/build/xltc
```

---

*For more information, see the main [README.md](../README.md) and [impl/README.md](../impl/README.md).*