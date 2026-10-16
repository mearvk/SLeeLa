#!/usr/bin/env bash
set -u
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
BIN="${SLEELA_BIN:-$ROOT/../impl/build/sleela}"
LIB="$ROOT/../lib"
fail=0
count=0
if [[ ! -x "$BIN" ]]; then echo "SLeeLa executable not found: $BIN" >&2; exit 2; fi
if [[ ! -d "$LIB" ]]; then echo "SLeeLa /lib not found: $LIB" >&2; exit 2; fi
while IFS= read -r -d '' src; do
  count=$((count+1))
  echo "SLVM conformance: $src"
  "$BIN" run "$src" >/dev/null 2>&1 || { echo "FAIL: $src" >&2; fail=1; }
done < <(find "$LIB" -type f -name '*.sleela' -print0 | sort -z)
echo "Checked $count /lib SLeeLa source files"
exit "$fail"