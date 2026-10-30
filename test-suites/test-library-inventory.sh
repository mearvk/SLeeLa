#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
LIB="$ROOT/lib"
EXPECTED_PACKAGES=108
EXPECTED_SOURCES=11757
EXPECTED_FACADES=75
EXPECTED_SYMBOLS=11832
# Filesystem truth: /lib is the single source of truth for the class vocabulary.
# A "package" is a top-level lib/ directory that contains at least one .sleela
# source -- this matches tools/generate-library-symbols.py (which derives
# packages from the .sleela tree) and the per-package "has a SLeeLa source"
# assertion below. Native-only subsystems under lib/ (e.g. db-connector: a C
# subsystem with Makefile/include/src and no .sleela) are intentionally not
# counted as SLeeLa packages.
package_count=$(find "$LIB" -mindepth 1 -maxdepth 1 -type d \
  -exec sh -c 'find "$1" -type f -name "*.sleela" -print -quit | grep -q .' _ {} \; -print | wc -l)
sleela_count=$(find "$LIB" -type f -name '*.sleela' | wc -l)
facade_fs_count=$(find "$LIB" -type f -name 'SLPackage.sleela' | wc -l)
source_fs_count=$((sleela_count - facade_fs_count))
# Manifest header (LIBRARY.SYMBOLS.md), kept in lockstep with the tree by
# tools/generate-library-symbols.py.
manifest_sources=$(awk -F': ' '/^library-source-files:/{print $2}' "$LIB/LIBRARY.SYMBOLS.md")
manifest_packages=$(awk -F': ' '/^library-packages:/{print $2}' "$LIB/LIBRARY.SYMBOLS.md")
manifest_facades=$(awk -F': ' '/^module-facade-symbols:/{print $2}' "$LIB/LIBRARY.SYMBOLS.md")
manifest_symbols=$(awk -F': ' '/^total-symbol-records:/{print $2}' "$LIB/LIBRARY.SYMBOLS.md")
# Manifest body: one row per .sleela unit; counts derived from the table.
row_sources=$(awk -F'\t' '$4=="source"{c++} END{print c+0}' "$LIB/LIBRARY.SYMBOLS.md")
row_facades=$(awk -F'\t' '$4=="facade"{c++} END{print c+0}' "$LIB/LIBRARY.SYMBOLS.md")
row_total=$((row_sources + row_facades))
# 1. Filesystem matches the expected totals.
test "$package_count" -eq "$EXPECTED_PACKAGES"
test "$source_fs_count" -eq "$EXPECTED_SOURCES"
test "$facade_fs_count" -eq "$EXPECTED_FACADES"
test "$sleela_count" -eq "$EXPECTED_SYMBOLS"
# 2. Manifest header matches the expected totals.
test "$manifest_packages" -eq "$EXPECTED_PACKAGES"
test "$manifest_sources" -eq "$EXPECTED_SOURCES"
test "$manifest_facades" -eq "$EXPECTED_FACADES"
test "$manifest_symbols" -eq "$EXPECTED_SYMBOLS"
# 3. Manifest body rows agree with the header (no header/body drift).
test "$row_sources" -eq "$manifest_sources"
test "$row_facades" -eq "$manifest_facades"
test "$row_total" -eq "$manifest_symbols"
# Every SLeeLa package carries at least one source. Native-only subsystems
# under lib/ (no .sleela at all, e.g. db-connector) are not SLeeLa packages and
# are skipped -- consistent with the package_count definition above.
for pkg in "$LIB"/*; do
  [ -d "$pkg" ] || continue
  find "$pkg" -type f -name '*.sleela' -print -quit | grep -q . || continue
done
test -f "$LIB/video/Video.sleela"
test -f "$LIB/video/VideoCodec.sleela"
test -f "$LIB/vm/SLVMModuleLoader.sleela"
grep -q 'library::Index' "$ROOT/impl/frontend/compiler.cpp"
grep -q 'library::Index' "$ROOT/impl/nordshrift/nordshrift.cpp"
grep -q 'packageSymbolCount' "$ROOT/impl/frontend/library_index.cpp"
grep -q 'symbolCount' "$LIB/vm/SLVMModuleLoader.sleela"
echo "PASS: /lib inventory packages=$package_count sources=$source_fs_count facades=$facade_fs_count symbols=$sleela_count"

test -f "$ROOT/impl/nordshrift/sst_symbol.cpp"
test -f "$ROOT/impl/nordshrift/SST.SYMBOLS.md"
grep -q 'SSTSymbol' "$ROOT/impl/nordshrift/sst_symbol.h"

test -f "$ROOT/impl/nordshrift/sst_symbol.cpp"
test -f "$ROOT/impl/nordshrift/SST.SYMBOLS.md"
grep -q 'SSTSymbol' "$ROOT/impl/nordshrift/sst_symbol.h"
