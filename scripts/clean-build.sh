#!/bin/bash

# =============================================================================
# clean-build.sh — Clean all build artifacts and rebuild from scratch
# =============================================================================
#
# Usage:
#   ./clean-build.sh              # Full clean rebuild
#   ./clean-build.sh --no-test    # Skip running tests after build
#
# Environment variables (optional):
#   CC              C compiler (default: gcc)
#   CXX             C++ compiler (default: g++)
#   BUILD_TYPE      Build type (default: Release with -O2)
#
# =============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
IMPL_DIR="$REPO_ROOT/impl"

export CC="${CC:-gcc}"
export CXX="${CXX:-g++}"
RUN_TESTS=true

# Parse arguments
for arg in "$@"; do
    case "$arg" in
        --no-test)
            RUN_TESTS=false
            ;;
    esac
done

echo "=========================================="
echo "SLeeLa Clean Build"
echo "=========================================="
echo "Repository:  $REPO_ROOT"
echo "Impl Dir:    $IMPL_DIR"
echo "C Compiler:  $CC"
echo "C++ Compiler: $CXX"
echo "Run Tests:   $RUN_TESTS"
echo "========================================"
echo ""

cd "$IMPL_DIR"

echo "Step 1: Clean all build artifacts..."
make clean
echo "✓ Clean complete"
echo ""

echo "Step 2: Building Sleela and Nordshrift..."
make all
echo "✓ Build complete"
echo ""

if [ ! -f "$IMPL_DIR/build/sleela" ] || [ ! -f "$IMPL_DIR/build/nordshrift" ]; then
    echo "ERROR: Build failed. Binaries not found in $IMPL_DIR/build/"
    exit 1
fi

echo "=========================================="
echo "Binaries"
echo "=========================================="
ls -lh "$IMPL_DIR/build/sleela" "$IMPL_DIR/build/nordshrift"
echo ""

if [ "$RUN_TESTS" = true ]; then
    echo "Step 3: Running test suite..."
    make test
    echo ""
    echo "✓ All tests passed"
fi

echo "=========================================="
echo "✓ Clean Build Complete"
echo "=========================================="
echo ""
echo "Binaries are ready in: $IMPL_DIR/build/"
echo ""
echo "Next steps:"
echo "  - Linux:  sudo ./scripts/build-linux.sh"
echo "  - macOS:  sudo ./scripts/build-macos.sh"
echo "  - Manual: cp $IMPL_DIR/build/{sleela,nordshrift} /usr/local/bin/"
echo ""
