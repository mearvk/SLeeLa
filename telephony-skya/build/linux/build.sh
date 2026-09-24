#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$ROOT/native"
OUT="$ROOT/build/linux"
CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:--std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread}"
if ! command -v "$CXX" >/dev/null 2>&1; then
 echo "C++ compiler not found: $CXX" >&2
 exit 1
fi
mkdir -p "$OUT"
"$CXX" $CXXFLAGS "$SRC/main.cpp" "$SRC/skya_engine.cpp" -o "$OUT/skya"
"$CXX" $CXXFLAGS "$SRC/skya_server_main.cpp" "$SRC/skya_engine.cpp" -o "$OUT/skya-server"
chmod +x "$OUT/skya" "$OUT/skya-server"
echo "Built: $OUT/skya"
echo "Built: $OUT/skya-server"
