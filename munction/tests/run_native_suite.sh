#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK="$ROOT/tests/.build"
CC="${CC:-cc}"
CXX="${CXX:-c++}"
rm -rf "$WORK"; mkdir -p "$WORK"
"$CC" -std=c11 -Wall -Wextra -Werror -I"$ROOT/c" "$ROOT/c/munction.c" "$ROOT/tests/c_smoke.c" -o "$WORK/c_smoke"
"$WORK/c_smoke"
"$CC" -std=c11 -Wall -Wextra -Werror -I"$ROOT/c" -c "$ROOT/c/munction.c" -o "$WORK/munction.o"
"$CXX" -std=c++17 -Wall -Wextra -Werror -I"$ROOT/c" -I"$ROOT/cpp" "$ROOT/cpp/munction.cpp" "$ROOT/tests/cpp_smoke.cpp" "$WORK/munction.o" -o "$WORK/cpp_smoke"
"$WORK/cpp_smoke"
echo "Munction native suite: PASS"
