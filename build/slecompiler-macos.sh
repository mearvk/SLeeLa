#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
SRC="$ROOT/decompiler"
OUT="$ROOT/build/slecompiler/macos"
BUILD="$OUT/cmake"
CMAKE="${CMAKE:-cmake}"
CONFIGURATION="${CONFIGURATION:-Release}"
MACOSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET:-11.0}"
CMAKE_OSX_ARCHITECTURES="${CMAKE_OSX_ARCHITECTURES:-$(uname -m)}"

command -v "$CMAKE" >/dev/null 2>&1 || { echo "ERROR: CMake 3.20+ not found." >&2; exit 1; }
command -v xcrun >/dev/null 2>&1 || { echo "ERROR: Xcode Command Line Tools are required (xcrun not found)." >&2; exit 1; }
[ -f "$SRC/CMakeLists.txt" ] || { echo "ERROR: Slecompiler CMake project not found at $SRC." >&2; exit 1; }

mkdir -p "$OUT"
"$CMAKE" -S "$SRC" -B "$BUILD" \
    -DCMAKE_BUILD_TYPE="$CONFIGURATION" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="$MACOSX_DEPLOYMENT_TARGET" \
    -DCMAKE_OSX_ARCHITECTURES="$CMAKE_OSX_ARCHITECTURES"
"$CMAKE" --build "$BUILD" --config "$CONFIGURATION"

echo "Slecompiler macOS build: $BUILD"
echo "Architecture: $CMAKE_OSX_ARCHITECTURES"
echo "Minimum macOS deployment target: $MACOSX_DEPLOYMENT_TARGET"
