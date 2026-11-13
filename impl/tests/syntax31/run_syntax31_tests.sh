#!/usr/bin/env bash
# Syntax 3.1 (SLEELA.syntax section 44): character literal, array initializer, labeled
# statements, abrupt completion through finally. Each <name>.sleela has a
# <name>.expected.txt; gate_* and bad_* must be rejected with the stated message.
# usage: run_syntax31_tests.sh [path-to-sleela]
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
  case "$n" in
    gate_char_in_3_0)       want="character literal requires #sleela 3.1" ;;
    gate_label_in_3_0)      want="labeled statement requires #sleela 3.1" ;;
    gate_arrayinit_in_3_0)  want="array initializer requires #sleela 3.1" ;;
    bad_label)              want="break to undefined label 'nowhere'" ;;
    *) want="" ;;
  esac
  if [ -n "$want" ]; then
    out="$("$SLEELA" check "$f" 2>&1 | filter)"
    if printf '%s' "$out" | grep -qF "$want"; then echo "PASS  $n (rejected: $want)"; pass=$((pass+1)); else echo "FAIL  $n: expected '$want'"; echo "$out" | head -5; fail=$((fail+1)); fi
    continue
  fi
  out="$("$SLEELA" run "$f" 2>&1 | filter)"
  if [ "$out" = "$(cat "$HERE/$n.expected.txt")" ]; then echo "PASS  $n"; pass=$((pass+1)); else echo "FAIL  $n"; diff <(printf '%s\n' "$out") "$HERE/$n.expected.txt" | head -20; fail=$((fail+1)); fi
done
echo "syntax 3.1 regression: pass=$pass fail=$fail"
[ "$fail" -eq 0 ] && echo "syntax 3.1 regression: PASS"
[ "$fail" -eq 0 ]
