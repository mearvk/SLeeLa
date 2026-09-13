#!/bin/bash

# =============================================================================
# build-macos.sh — Build SLeeLa for macOS and install to /usr/local/bin
# =============================================================================
# 
# Usage:
#   ./build-macos.sh              # Build with default flags
#   CC=clang CXX=clang++ ./build-macos.sh
#   BIN_DIR=/usr/local/bin ./build-macos.sh
#
# Environment variables (optional):
#   CC              C compiler (default: clang)
#   CXX             C++ compiler (default: clang++)
#   BIN_DIR         Installation directory (default: /usr/local/bin)
#   CFLAGS          Custom C compiler flags
#   CXXFLAGS        Custom C++ compiler flags
#
# Requirements:
#   - Xcode Command Line Tools (xcode-select --install)
#   - C11-compatible C compiler (clang)
#   - C++17-compatible C++ compiler
#   - make
#
# =============================================================================

set -e  # Exit on error

# Configuration
BIN_DIR="${BIN_DIR:-/usr/local/bin}"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
IMPL_DIR="$REPO_ROOT/impl"
BUILD_DIR="$IMPL_DIR/build"

# Set default compilers if not already set (macOS typically uses clang)
export CC="${CC:-clang}"
export CXX="${CXX:-clang++}"

# Print configuration
echo "=========================================="
echo "SLeeLa macOS Build Configuration"
echo "=========================================="
echo "C Compiler:       $CC"
echo "C++ Compiler:     $CXX"
echo "Installation Dir: $BIN_DIR"
echo "Build Dir:        $BUILD_DIR"
echo "========================================"
echo ""

# Check for required tools
check_command() {
    if ! command -v "$1" &> /dev/null; then
        echo "ERROR: '$1' not found. Please install it and try again."
        if [ "$1" == "clang" ] || [ "$1" == "clang++" ]; then
            echo "  Install Xcode Command Line Tools: xcode-select --install"
        fi
        exit 1
    fi
}

check_command "$CC"
check_command "$CXX"
check_command "make"

# Check C compiler version
echo "Checking C compiler capabilities..."
if $CC --version | grep -q "clang"; then
    $CC --version | head -1
else
    echo "WARNING: Non-clang compiler detected on macOS."
fi

# Check C++ compiler version
echo "Checking C++ compiler capabilities..."
if $CXX --version | grep -q "clang"; then
    $CXX --version | head -1
else
    echo "WARNING: Non-clang++ compiler detected on macOS."
fi

echo ""

# Build
echo "Building Sleela and Nordshrift..."
cd "$IMPL_DIR"
make clean
make all

if [ ! -f "$BUILD_DIR/sleela" ] || [ ! -f "$BUILD_DIR/nordshrift" ]; then
    echo "ERROR: Build failed. Binaries not found."
    exit 1
fi

echo ""
echo "Build completed successfully!"
echo ""

# Verify binaries
echo "=========================================="
echo "Generated Binaries"
echo "=========================================="
ls -lh "$BUILD_DIR/sleela" "$BUILD_DIR/nordshrift"
echo ""

# Check if we have write permission to BIN_DIR
if [ "$BIN_DIR" != "$BUILD_DIR" ]; then
    if [ ! -d "$BIN_DIR" ]; then
        echo "ERROR: Installation directory '$BIN_DIR' does not exist."
        echo "Create it first, or set BIN_DIR to an existing directory."
        exit 1
    fi
    
    if [ ! -w "$BIN_DIR" ]; then
        echo "ERROR: No write permission to '$BIN_DIR'."
        echo "Try running with sudo or set BIN_DIR to a writable directory:"
        echo "  BIN_DIR=\$HOME/bin ./build-macos.sh"
        exit 1
    fi

    echo "Installing binaries to $BIN_DIR..."
    cp "$BUILD_DIR/sleela" "$BIN_DIR/sleela"
    cp "$BUILD_DIR/nordshrift" "$BIN_DIR/nordshrift"
    chmod +x "$BIN_DIR/sleela" "$BIN_DIR/nordshrift"
    
    echo "=========================================="
    echo "Installation Complete"
    echo "=========================================="
    echo "sleela:     $BIN_DIR/sleela"
    echo "nordshrift: $BIN_DIR/nordshrift"
    echo ""
    echo "Verify installation:"
    echo "  $BIN_DIR/sleela version"
    echo "  $BIN_DIR/nordshrift version"
else
    echo "=========================================="
    echo "Build Complete (no separate install)"
    echo "=========================================="
    echo "Binaries remain in: $BUILD_DIR"
    echo ""
    echo "To use globally, copy them to a directory in \$PATH:"
    echo "  sudo cp $BUILD_DIR/sleela /usr/local/bin/"
    echo "  sudo cp $BUILD_DIR/nordshrift /usr/local/bin/"
    echo ""
    echo "Or set BIN_DIR and run again:"
    echo "  BIN_DIR=/usr/local/bin ./build-macos.sh"
fi

echo ""
echo "Build process finished."
