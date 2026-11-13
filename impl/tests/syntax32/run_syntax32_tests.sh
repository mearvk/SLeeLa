#!/usr/bin/env bash
# Syntax 3.2 (SLEELA.syntax sections 45 char, 46 array). Each <name>.sleela has a
# <name>.expected.txt; gate_* must be rejected at check time with the stated message,
# bad_* must fail (check or run) with the stated message.
# usage: run_syntax32_tests.sh [path-to-sleela]
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$HERE/../../.." && pwd)"
SLEELA="${1:-$ROOT/impl/build/sleela}"
case "$SLEELA" in /*) ;; *) SLEELA="$(cd "$(dirname "$SLEELA")" && pwd)/$(basename "$SLEELA")" ;; esac
export SLEELA_HOME="$ROOT" SLEELA_LIB="$ROOT/lib" SLEELA_SHEET="$ROOT/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"
cd "$ROOT"
filter() { grep -v 'memory-manager\|^verification\|\[security\]\|\[defender\]'; }
pass=0; fail=0
for f in "$HERE"/*.sleela; do
  n="$(basename "$f" .sleela)"
  want=""; mode=check
  case "$n" in
    gate_length_in_3_1)          want="the array length member requires #sleela 3.2" ;;
    gate_nd_in_3_1)              want="multi-dimensional array creation requires #sleela 3.2" ;;
    bad_nd_sized_after_unsized)  want="array dimension expression after an empty" ;;
    bad_nd_negative)             want="negative array size"; mode=run ;;
    bad_length_assign)           want="the array length is read-only" ;;
    bad_length_incr)             want="the array length is read-only" ;;
  esac
  if [ -n "$want" ]; then
    out="$("$SLEELA" $mode "$f" 2>&1 | filter)"
    if printf '%s' "$out" | grep -qF "$want"; then echo "PASS  $n (rejected: $want)"; pass=$((pass+1)); else echo "FAIL  $n: expected '$want'"; echo "$out" | head -5; fail=$((fail+1)); fi
    continue
  fi
  out="$("$SLEELA" run "$f" 2>&1 | filter)"
  if [ "$out" = "$(cat "$HERE/$n.expected.txt")" ]; then echo "PASS  $n"; pass=$((pass+1)); else echo "FAIL  $n"; diff <(printf '%s\n' "$out") "$HERE/$n.expected.txt" | head -20; fail=$((fail+1)); fi
done
echo "syntax 3.2 regression: pass=$pass fail=$fail"
[ "$fail" -eq 0 ] && echo "syntax 3.2 regression: PASS"
[ "$fail" -eq 0 ]
