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
XLT_DIR="$IMPL_DIR/xlt"
XLT_BUILD_DIR="$XLT_DIR/build"

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

echo "Step 3: Clean-building the XLT compiler (xltc)..."
make -C "$XLT_DIR" clean
make -C "$XLT_DIR"
echo "✓ XLT build complete"
echo ""

if [ ! -f "$XLT_BUILD_DIR/xltc" ]; then
    echo "ERROR: XLT build failed. 'xltc' not found in $XLT_BUILD_DIR"
    exit 1
fi

echo "=========================================="
echo "Binaries"
echo "=========================================="
ls -lh "$IMPL_DIR/build/sleela" "$IMPL_DIR/build/nordshrift" "$XLT_BUILD_DIR/xltc"
echo ""

if [ "$RUN_TESTS" = true ]; then
    echo "Step 4: Running test suite..."
    make test
    echo ""

    # XLT compiler smoke test, both directions ("for and against"):
    #   - a congruent model (epyc-ddr2.xlt) MUST compile (exit 0)
    #   - a deliberately bad model (bad-model.xlt) MUST be rejected (non-zero)
    echo "Step 5: XLT compiler smoke test (for and against)..."
    XLT_EX="$REPO_ROOT/lib/xlt/examples"
    if "$XLT_BUILD_DIR/xltc" "$XLT_EX/epyc-ddr2.xlt" >/dev/null 2>&1; then
        echo "✓ xltc accepts a congruent model (epyc-ddr2.xlt)"
    else
        echo "ERROR: xltc rejected a model that should compile: epyc-ddr2.xlt"
        exit 1
    fi
    if [ -f "$XLT_EX/bad-model.xlt" ]; then
        if "$XLT_BUILD_DIR/xltc" "$XLT_EX/bad-model.xlt" >/dev/null 2>&1; then
            echo "ERROR: xltc accepted a model that should be rejected: bad-model.xlt"
            exit 1
        else
            echo "✓ xltc rejects a bad model (bad-model.xlt)"
        fi
    fi
    echo ""
    echo "✓ All tests passed"
fi

echo "=========================================="
echo "✓ Clean Build Complete"
echo "=========================================="
echo ""
echo "Binaries are ready in: $IMPL_DIR/build/ (sleela, nordshrift)"
echo "XLT compiler is ready in: $XLT_BUILD_DIR/ (xltc)"
echo ""
echo "Next steps:"
echo "  - Linux:  sudo ./scripts/build-linux.sh"
echo "  - macOS:  sudo ./scripts/build-macos.sh"
echo "  - Manual: cp $IMPL_DIR/build/{sleela,nordshrift} /usr/local/bin/"
echo "            cp $XLT_BUILD_DIR/xltc /usr/local/bin/"
echo ""
