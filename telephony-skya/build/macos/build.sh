#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname "$0")/../.." && pwd)
SRC="$ROOT/telephony-skya/native"
OUT="$ROOT/telephony-skya/build/macos"
CXX="${CXX:-clang++}"
CXXFLAGS="${CXXFLAGS:--std=c++17 -O2 -Wall -Wextra -Wpedantic}"
command -v "$CXX" >/dev/null 2>&1 || { echo "C++ compiler not found: $CXX" >&2; exit 1; }
mkdir -p "$OUT"
"$CXX" $CXXFLAGS "$SRC/main.cpp" "$SRC/skya_engine.cpp" -o "$OUT/skya"
chmod +x "$OUT/skya"
echo "Built: $OUT/skya"
