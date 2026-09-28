# Build Scripts for SLeeLa

This directory contains build and installation scripts for SLeeLa.

## Available Scripts

### `build-linux.sh` — Linux Build & Install
Builds SLeeLa for Linux (gcc/g++) and installs to `/bin` or a custom directory.

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
```

---

*For more information, see the main [README.md](../README.md) and [impl/README.md](../impl/README.md).*
