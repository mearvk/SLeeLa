#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
SRC="$ROOT/decompiler"
OUT="$ROOT/build/slecompiler/linux"
BUILD="$OUT/cmake"
CMAKE="${CMAKE:-cmake}"
CONFIGURATION="${CONFIGURATION:-Release}"
command -v "$CMAKE" >/dev/null 2>&1 || { echo "ERROR: CMake 3.20+ not found." >&2; exit 1; }
[ -f "$SRC/CMakeLists.txt" ] || { echo "ERROR: Slecompiler CMake project not found at $SRC." >&2; exit 1; }
mkdir -p "$OUT"
"$CMAKE" -S "$SRC" -B "$BUILD" -DCMAKE_BUILD_TYPE="$CONFIGURATION"
"$CMAKE" --build "$BUILD" --config "$CONFIGURATION"
echo "Slecompiler Linux build: $BUILD"
