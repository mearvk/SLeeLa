#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
LIB="$ROOT/lib"
EXPECTED_PACKAGES=77
EXPECTED_SOURCES=10046
EXPECTED_FACADES=90
EXPECTED_SYMBOLS=10136
package_count=$(find "$LIB" -mindepth 1 -maxdepth 1 -type d | wc -l)
source_count=$(find "$LIB" -type f -name '*.sleela' | wc -l)
manifest_sources=$(awk -F': ' '/^library-source-files:/{print $2}' "$LIB/LIBRARY.SYMBOLS.md")
manifest_packages=$(awk -F': ' '/^library-packages:/{print $2}' "$LIB/LIBRARY.SYMBOLS.md")
manifest_facades=$(awk -F': ' '/^module-facade-symbols:/{print $2}' "$LIB/LIBRARY.SYMBOLS.md")
manifest_symbols=$(awk -F': ' '/^total-symbol-records:/{print $2}' "$LIB/LIBRARY.SYMBOLS.md")
test "$package_count" -eq "$EXPECTED_PACKAGES"
test "$source_count" -eq "$EXPECTED_SOURCES"
test "$manifest_packages" -eq "$EXPECTED_PACKAGES"
test "$manifest_sources" -eq "$EXPECTED_SOURCES"
test "$manifest_facades" -eq "$EXPECTED_FACADES"
test "$manifest_symbols" -eq "$EXPECTED_SYMBOLS"
for pkg in "$LIB"/*; do
  [ -d "$pkg" ] || continue
  find "$pkg" -type f -name '*.sleela' -print -quit | grep -q . || { echo "FAIL: package $(basename "$pkg") has no SLeeLa source"; exit 1; }
done
test -f "$LIB/video/Video.sleela"
test -f "$LIB/video/VideoCodec.sleela"
test -f "$LIB/vm/SLVMModuleLoader.sleela"
grep -q 'library::Index' "$ROOT/impl/frontend/compiler.cpp"
grep -q 'library::Index' "$ROOT/impl/nordshrift/nordshrift.cpp"
grep -q 'packageSymbolCount' "$ROOT/impl/frontend/library_index.cpp"
grep -q 'symbolCount' "$LIB/vm/SLVMModuleLoader.sleela"
echo "PASS: /lib inventory packages=$package_count sources=$source_count facades=$manifest_facades symbols=$manifest_symbols"

test -f "$ROOT/impl/nordshrift/sst_symbol.cpp"
test -f "$ROOT/impl/nordshrift/SST.SYMBOLS.md"
grep -q 'SSTSymbol' "$ROOT/impl/nordshrift/sst_symbol.h"

test -f "$ROOT/impl/nordshrift/sst_symbol.cpp"
test -f "$ROOT/impl/nordshrift/SST.SYMBOLS.md"
grep -q 'SSTSymbol' "$ROOT/impl/nordshrift/sst_symbol.h"
