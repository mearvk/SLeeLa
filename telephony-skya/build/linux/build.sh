#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
REPO_ROOT="$(cd "$ROOT/.." && pwd)"
SRC="$ROOT/native"; OUT="$ROOT/build/linux"; ASSETS="$OUT/assets"; LOGO="$REPO_ROOT/images/skya-logo-blue.jpeg"
CXX="${CXX:-g++}"; CXXFLAGS="${CXXFLAGS:--std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread}"
command -v "$CXX" >/dev/null 2>&1 || { echo "C++ compiler not found: $CXX" >&2; exit 1; }
[ -f "$LOGO" ] || { echo "Skya logo not found: $LOGO" >&2; exit 1; }
mkdir -p "$OUT" "$ASSETS"
"$CXX" $CXXFLAGS "$SRC/main.cpp" "$SRC/skya_engine.cpp" -o "$OUT/skya"
"$CXX" $CXXFLAGS "$SRC/skya_server_main.cpp" "$SRC/skya_engine.cpp" -o "$OUT/skya-server"
cp "$LOGO" "$ASSETS/skya-logo-blue.jpeg"; chmod +x "$OUT/skya" "$OUT/skya-server"
echo "Built: $OUT/skya"; echo "Built: $OUT/skya-server"; echo "Brand asset: $ASSETS/skya-logo-blue.jpeg"
