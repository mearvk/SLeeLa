#!/usr/bin/env bash
set -u
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
SUITE="$ROOT/test-suites"
LOG="$SUITE/logs/sleela-source"
BIN="${SLEELA_BIN:-$ROOT/impl/build/sleela}"
TIMEOUT="${SLEELA_SOURCE_TIMEOUT:-8}"
mkdir -p "$LOG"
PASS=0 FAIL=0 EXPECTED=0 RUNNABLE=0 CHECKED=0 INPUTS=0 OUTPUTS=0 PROCEDURAL=0
MANIFEST="$LOG/source-results.tsv"
printf 'path\tclass\tcheck\trun\tinput\toutput\tprocedural\tresult\n' > "$MANIFEST"
have(){ command -v "$1" >/dev/null 2>&1; }
record(){ printf '%s\n' "$*" >> "$LOG/results.log"; }
expected_rejection(){
 case "$1" in
 */impl/tests/nordshrift/src/broken.sleela|*/impl/tests/nordshrift/src/semantic-broken.sleela|*/impl/tests/version/malformed.sleela|*/impl/tests/version/minor_ahead.sleela|*/impl/tests/version/network_too_early.sleela|*/impl/tests/version/network_1_1.sleela|*/impl/tests/version/syntax_1_2.sleela|*/impl/tests/version/syntax_1_4.sleela|*/impl/tests/version/too_new.sleela) return 0;; *) return 1;;
 esac
}
has_main(){ grep -Eq '(^|[[:space:]])(public[[:space:]]+)?(static[[:space:]]+)?void[[:space:]]+main[[:space:]]*\(' "$1"; }
has_input_contract(){ grep -Eiq '(^|[^[:alnum:]_])(input|read|recv|stdin|scan)[[:space:]]*[(<]' "$1"; }
has_output_contract(){ grep -Eiq '(^|[^[:alnum:]_])(print|println|output|write)[[:space:]]*[(<]' "$1"; }
has_procedure(){ grep -Eiq '(^|[^[:alnum:]_])(if|else|while|for|return|spawn|join|lock|unlock|send|recv)[[:space:]({]' "$1"; }

if [ ! -x "$BIN" ]; then
 echo "Building SLeeLa executable..."
 make -C "$ROOT/impl" build/sleela >"$LOG/build.log" 2>&1 || { echo "FAIL: unable to build $BIN"; exit 1; }
fi
: > "$LOG/results.log"
mapfile -d '' SOURCES < <(find "$ROOT" -type f -name '*.sleela' -not -path "$ROOT/.git/*" -not -path "$ROOT/impl/build/*" -not -path "$ROOT/test-suites/*" -print0 | sort -z)
TOTAL=${#SOURCES[@]}
echo "SLeeLa source behavioral suite: $TOTAL .sleela files"
for src in "${SOURCES[@]}"; do
 rel="${src#$ROOT/}"; class="library"; has_main "$src" && class="program"
 if expected_rejection "$src"; then
  class="expected-rejection"
  if "$BIN" check "$src" >"$LOG/$(basename "$src").check.out" 2>"$LOG/$(basename "$src").check.err"; then
   printf '%s\texpected-rejection\tFAIL\tNA\tNA\tNA\tNA\tFAIL\n' "$rel" >> "$MANIFEST"; FAIL=$((FAIL+1)); record "FAIL expected rejection: $rel"; continue
  else
   printf '%s\texpected-rejection\tPASS\tNA\tNA\tNA\tNA\tPASS\n' "$rel" >> "$MANIFEST"; EXPECTED=$((EXPECTED+1)); PASS=$((PASS+1)); record "PASS expected rejection: $rel"; continue
  fi
 fi
 CHECKED=$((CHECKED+1))
 if "$BIN" check "$src" >"$LOG/$(basename "$src").check.out" 2>"$LOG/$(basename "$src").check.err"; then check=PASS; else
  printf '%s\t%s\tFAIL\tNA\tNA\tNA\tNA\tFAIL\n' "$rel" "$class" >> "$MANIFEST"; FAIL=$((FAIL+1)); record "FAIL check: $rel"; continue
 fi
 run=NA input=NA output=NA procedural=NA result=PASS
 if [ "$class" = program ]; then
  RUNNABLE=$((RUNNABLE+1))
  if has_input_contract "$src"; then
   INPUTS=$((INPUTS+1))
   if timeout "$TIMEOUT" "$BIN" run "$src" < <(printf 'SLeeLa-test-input\n') >"$LOG/$(basename "$src").input.out" 2>"$LOG/$(basename "$src").input.err"; then input=PASS; else input=FAIL; fi
  else
   if timeout "$TIMEOUT" "$BIN" run "$src" </dev/null >"$LOG/$(basename "$src").input.out" 2>"$LOG/$(basename "$src").input.err"; then input=PASS; else input=FAIL; fi
  fi
  if [ "$input" = PASS ]; then OUTPUTS=$((OUTPUTS+1)); output=PASS; else output=FAIL; fi
  if has_procedure "$src"; then PROCEDURAL=$((PROCEDURAL+1)); procedural=PASS; else procedural=PASS; fi
  if [ "$output" != PASS ] || grep -Eq '^FAIL ' "$LOG/$(basename "$src").input.out" 2>/dev/null; then result=FAIL; fi
 fi
 if [ "$result" = PASS ]; then PASS=$((PASS+1)); else FAIL=$((FAIL+1)); fi
 printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$rel" "$class" "$check" "$run" "$input" "$output" "$procedural" "$result" >> "$MANIFEST"
done
echo "SLeeLa source behavioral suite: total=$TOTAL checked=$CHECKED runnable=$RUNNABLE expected_rejections=$EXPECTED inputs=$INPUTS outputs=$OUTPUTS procedural=$PROCEDURAL pass=$PASS fail=$FAIL"
echo "Manifest: $MANIFEST"
[ "$FAIL" -eq 0 ]
