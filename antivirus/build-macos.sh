#!/bin/sh
# Sleelavirin(TM) native engine - macOS build.
# Max Rupplin - MEARVK LLC - 2026.
#
# Builds the C library, the C++ library, and the sleelavirin-scan CLI with the
# Apple/Clang toolchain into ./build. Portable C11 / C++17; no third-party deps.
#
# Usage:
#   ./build-macos.sh            # build libraries + scanner
#   ./build-macos.sh test       # build then run the test suite
#
# Environment:
#   CC, CXX                     compilers (default: clang, clang++)
#   MACOSX_DEPLOYMENT_TARGET    e.g. 10.9 to target older macOS (optional)
#
# A CMake build also works and is preferred when available:
#   cmake -S . -B build -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9 && cmake --build build
set -eu

ROOT=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
cd "$ROOT"

CC="${CC:-clang}"
CXX="${CXX:-clang++}"
CFLAGS="${CFLAGS:--O2 -Wall -Wextra -Wpedantic -std=c11 -Isrc}"
CXXFLAGS="${CXXFLAGS:--O2 -Wall -Wextra -Wpedantic -std=c++17 -Isrc}"
BUILD="$ROOT/build"

if [ -n "${MACOSX_DEPLOYMENT_TARGET:-}" ]; then
    CFLAGS="$CFLAGS -mmacosx-version-min=$MACOSX_DEPLOYMENT_TARGET"
    CXXFLAGS="$CXXFLAGS -mmacosx-version-min=$MACOSX_DEPLOYMENT_TARGET"
fi

command -v "$CC" >/dev/null 2>&1 || { echo "ERROR: C compiler '$CC' not found (install Xcode command line tools)." >&2; exit 1; }
command -v "$CXX" >/dev/null 2>&1 || { echo "ERROR: C++ compiler '$CXX' not found." >&2; exit 1; }

mkdir -p "$BUILD"

echo "== Compiling C engine =="
OBJS=""
for m in scan heuristic native ac sig bridge; do
    $CC $CFLAGS -c "src/sleelavirin_$m.c" -o "$BUILD/sleelavirin_$m.o"
    OBJS="$OBJS $BUILD/sleelavirin_$m.o"
done
ar rcs "$BUILD/libsleelavirin.a" $OBJS
echo "  -> build/libsleelavirin.a"

echo "== Compiling C++ wrapper =="
$CXX $CXXFLAGS -c src/sleelavirin_cpp.cpp -o "$BUILD/sleelavirin_cpp.o"
ar rcs "$BUILD/libsleelavirin_cpp.a" "$BUILD/sleelavirin_cpp.o"
echo "  -> build/libsleelavirin_cpp.a"

echo "== Linking scanner =="
$CC $CFLAGS src/sleelavirin_native_main.c "$BUILD/libsleelavirin.a" -o "$BUILD/sleelavirin-scan"
chmod +x "$BUILD/sleelavirin-scan"
echo "  -> build/sleelavirin-scan"

if [ "${1:-}" = "test" ]; then
    echo "== Tests =="
    $CC $CFLAGS tests/test_scan.c "$BUILD/libsleelavirin.a" -o "$BUILD/test_scan" && "$BUILD/test_scan"
    $CC $CFLAGS tests/test_ac.c "$BUILD/libsleelavirin.a" -o "$BUILD/test_ac" && "$BUILD/test_ac"
    $CC $CFLAGS tests/test_bridge.c "$BUILD/libsleelavirin.a" -o "$BUILD/test_bridge" && "$BUILD/test_bridge"
    $CXX $CXXFLAGS tests/test_cpp_scan.cpp "$BUILD/libsleelavirin_cpp.a" "$BUILD/libsleelavirin.a" -o "$BUILD/test_cpp_scan" && "$BUILD/test_cpp_scan"
fi

echo "Done."
