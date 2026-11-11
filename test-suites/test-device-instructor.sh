#!/usr/bin/env bash
# Assurance gate for the Device Instructor series
# (lib/advantage/learner-vs-professional/series).
#   1. every lesson folder has a lesson class with a main(), the Metadata block
#      header, and an .expected.txt file;
#   2. running each lesson reproduces its .expected.txt exactly;
#   3. the lesson set is the 32 lessons the series README promises.
# Set UPDATE=1 to rewrite the expected files after reviewing a deliberate change.
# Needs the sleela binary (default working-binaries/sleela, override SLEELA=...)
# and a security/sha256-manifest.json that matches the tree.
set -uo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
SLEELA="${SLEELA:-$ROOT/working-binaries/sleela}"
SERIES="lib/advantage/learner-vs-professional/series"
fail=0
bad() { echo "FAIL: $*" >&2; fail=1; }

lessons=("$SERIES"/[0-9][0-9]-*/)
[ "${#lessons[@]}" -eq 32 ] || bad "expected 32 lesson folders, found ${#lessons[@]}"
[ -f "$SERIES/SLNounRegistry.sleela" ] || bad "missing shared SLNounRegistry.sleela"
[ -f "$SERIES/SLDecisionDesk.sleela" ] || bad "missing shared SLDecisionDesk.sleela"

filter() { grep -v -e '^\[memory' -e '^\[security' -e '^\[defender' -e '^verification'; }

for dir in "${lessons[@]}"; do
  dir="${dir%/}"
  src="$(ls "$dir"/*.sleela 2>/dev/null | head -1)"
  [ -n "$src" ] || { bad "$dir has no lesson .sleela"; continue; }
  exp="${src%.sleela}.expected.txt"
  grep -q 'void main()' "$src" || bad "$src has no main()"
  grep -q 'Descriptive metadata (version 1.11 nested-friendly accessors)' "$src" || bad "$src lacks the Metadata block"
  out="$("$SLEELA" run "$src" 2>&1 | filter)"
  if [ "${UPDATE:-0}" = "1" ]; then
    printf '%s\n' "$out" > "$exp"; echo "updated $exp"; continue
  fi
  [ -f "$exp" ] || { bad "$src has no expected file"; continue; }
  if [ "$out" != "$(cat "$exp")" ]; then
    bad "$src output differs from $exp"
    diff <(printf '%s\n' "$out") "$exp" | head -10 >&2
  else
    echo "ok: $src"
  fi
done

[ "$fail" -eq 0 ] && echo "PASS: device instructor series (${#lessons[@]} lessons)"
exit "$fail"
