#!/usr/bin/env bash
# Assurance gate for lib/advantage/trading-actions-on-verb.
#   1. twenty-seven use-case folders, each with a class that has main(), the
#      Metadata block and an .expected.txt;
#   2. each run reproduces its .expected.txt exactly;
#   3. no run prints a FAIL line and every run ends with "failed=0".
# A case that does not finish within CASE_TIMEOUT seconds (default 300) fails
# instead of hanging the gate.
# UPDATE=1 rewrites the expected files after a reviewed change.
set -uo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
SLEELA="${SLEELA:-$ROOT/working-binaries/sleela}"
DIR="lib/advantage/trading-actions-on-verb"
fail=0
bad() { echo "FAIL: $*" >&2; fail=1; }
filter() { grep -v -e '^\[memory' -e '^\[security' -e '^\[defender' -e '^verification'; }

cases=("$DIR"/[0-9][0-9]-*/)
[ "${#cases[@]}" -eq 27 ] || bad "expected 27 use cases, found ${#cases[@]}"
[ -f "$DIR/SLLongBook.sleela" ] || bad "missing SLLongBook.sleela"
[ -f "$DIR/SLEuclid.sleela" ] || bad "missing SLEuclid.sleela"
[ -f "$DIR/SLShortBook.sleela" ] || bad "missing SLShortBook.sleela"
[ -f "$DIR/SLShortMargin.sleela" ] || bad "missing SLShortMargin.sleela"
[ -f "$DIR/SLBatchReturn.sleela" ] || bad "missing SLBatchReturn.sleela"
for lib in SLLongBook SLShortBook SLShortMargin SLEuclid SLBatchReturn; do
  grep -q 'Descriptive metadata (version 1.11 nested-friendly accessors)' "$DIR/$lib.sleela" || bad "$lib lacks the Metadata block"
done

for c in "${cases[@]}"; do
  c="${c%/}"
  src="$(ls "$c"/*.sleela 2>/dev/null | head -1)"
  [ -n "$src" ] || { bad "$c has no .sleela"; continue; }
  exp="${src%.sleela}.expected.txt"
  grep -q 'void main()' "$src" || bad "$src has no main()"
  grep -q 'Descriptive metadata (version 1.11 nested-friendly accessors)' "$src" || bad "$src lacks the Metadata block"
  out="$(timeout "${CASE_TIMEOUT:-300}" "$SLEELA" run "$src" 2>&1 | filter)"
  if [ "${UPDATE:-0}" = "1" ]; then printf '%s\n' "$out" > "$exp"; echo "updated $exp"; continue; fi
  printf '%s\n' "$out" | grep -q '^  FAIL ' && bad "$src printed a FAIL line"
  printf '%s\n' "$out" | tail -1 | grep -q 'failed=0$' || bad "$src did not end with failed=0"
  [ -f "$exp" ] || { bad "$src has no expected file"; continue; }
  if [ "$out" != "$(cat "$exp")" ]; then
    bad "$src output differs from $exp"; diff <(printf '%s\n' "$out") "$exp" | head -10 >&2
  else echo "ok: $src"; fi
done
[ "$fail" -eq 0 ] && echo "PASS: trading actions on verb (${#cases[@]} use cases)"
exit "$fail"
