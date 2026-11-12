#!/usr/bin/env bash
# Check every /jdk-28 SLeeLa file with the SLeeLa compiler (parse + resolve + semantics).
# A library file has no main(), so "no 'main' method found" is the only accepted error.
# usage: bash jdk-28/tools/check_jdk28.sh [jobs]   (run from the repository root)
set -euo pipefail
jobs="${1:-8}"
bin="${SLEELA_BIN:-working-binaries/sleela}"
report="${REPORT:-jdk-28/CHECK-REPORT.txt}"
export bin
find jdk-28 -name '*.sleela' -print0 | sort -z |
  xargs -0 -P "$jobs" -I{} sh -c '
    out=$("$bin" check "$1" 2>&1 | grep "error:" | grep -v "no .main. method found" || true)
    if [ -n "$out" ]; then printf "FAIL %s\n%s\n" "$1" "$out"; else printf "OK %s\n" "$1"; fi
  ' _ {} > "$report.tmp"
ok=$(grep -c '^OK ' "$report.tmp" || true)
fail=$(grep -c '^FAIL ' "$report.tmp" || true)
{ echo "jdk-28 check: $ok ok, $fail failed ($("$bin" version 2>/dev/null | grep -m1 SLeeLa))"; grep -v '^OK ' "$report.tmp" || true; } > "$report"
rm -f "$report.tmp"
head -1 "$report"
[ "$fail" -eq 0 ]
