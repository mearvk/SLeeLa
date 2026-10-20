#!/usr/bin/env sh
# Build the repository's portable native C/C++ targets on macOS.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$ROOT"
: "${CC:=clang}"
: "${CXX:=clang++}"
export CC CXX
command -v "$CC" >/dev/null 2>&1 || { echo "C compiler not found: $CC" >&2; exit 127; }
command -v "$CXX" >/dev/null 2>&1 || { echo "C++ compiler not found: $CXX" >&2; exit 127; }
echo "SLeeLa native build: macOS; CC=$CC; CXX=$CXX"
make -C impl clean all
make -C impl test-platform test-threads test-network
make -C regex test
make -C antivirus test
echo "macOS native build and selected smoke tests completed."
