#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
SRC="$ROOT/telephony-skya/native"
OUT="$ROOT/build/skya/linux"
CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:--std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread}"
command -v "$CXX" >/dev/null 2>&1 || { echo "ERROR: C++ compiler not found." >&2; exit 1; }
mkdir -p "$OUT"
"$CXX" $CXXFLAGS "$SRC/main.cpp" "$SRC/skya_engine.cpp" -o "$OUT/skya"
chmod +x "$OUT/skya"
echo "Skya Linux binary: $OUT/skya"
"$OUT/skya" --both --room "${SKYA_ROOM:-lobby}"
