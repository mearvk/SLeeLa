#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
SRC="$ROOT/decompiler"
OUT="$ROOT/build/slecompiler/linux"
BUILD="$OUT/cmake"
CMAKE="${CMAKE:-cmake}"
command -v "$CMAKE" >/dev/null 2>&1 || { echo "ERROR: CMake not found." >&2; exit 1; }
mkdir -p "$OUT"
"$CMAKE" -S "$SRC" -B "$BUILD" -DCMAKE_BUILD_TYPE=Release
"$CMAKE" --build "$BUILD" --config Release
echo "Slecompiler Linux build: $BUILD"
