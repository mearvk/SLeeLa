#!/usr/bin/env bash
# Runtime pressure tests: garbage collection under allocation churn, the bounded live-object
# tables (4096 arrays, 4096 structs), allocation while a finally block runs, and worker threads.
# <name>.sleela with <name>.expected.txt must print exactly that; limit_* must fail with the
# stated message (a full table is an error, never silent corruption).
# usage: run_pressure_tests.sh [path-to-sleela]
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$HERE/../../../.." && pwd)"
SLEELA="${1:-$ROOT/impl/build/sleela}"
case "$SLEELA" in /*) ;; *) SLEELA="$(cd "$(dirname "$SLEELA")" && pwd)/$(basename "$SLEELA")" ;; esac
export SLEELA_HOME="$ROOT" SLEELA_LIB="$ROOT/lib" SLEELA_SHEET="$ROOT/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"
cd "$ROOT"
filter() { grep -v 'memory-manager\|^verification\|\[security\]\|\[defender\]'; }
run_with_timeout() {
  python3 - "$SLEELA" "$1" <<'PY'
import subprocess
import sys

try:
    result = subprocess.run(
        [sys.argv[1], "run", sys.argv[2]],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        timeout=300,
        check=False,
    )
except subprocess.TimeoutExpired as exc:
    output = exc.stdout or b""
    if isinstance(output, str):
        output = output.encode()
    sys.stdout.buffer.write(output)
    sys.stderr.write("runtime pressure command timed out after 300 seconds\n")
    raise SystemExit(124)

sys.stdout.buffer.write(result.stdout or b"")
raise SystemExit(result.returncode)
PY
}
pass=0; fail=0
for f in "$HERE"/*.sleela; do
  n="$(basename "$f" .sleela)"
  out="$(run_with_timeout "$f" 2>&1 | filter)"
  case "$n" in
    limit_arrays)  want="the live-array table is full: at most 4096 arrays" ;;
    limit_structs) want="the live-struct table is full: at most 4096 struct instances" ;;
    *) want="" ;;
  esac
  if [ -n "$want" ]; then
    if printf '%s' "$out" | grep -qF "$want" && ! printf '%s' "$out" | grep -qF unreachable; then echo "PASS  $n (fails closed: $want)"; pass=$((pass+1)); else echo "FAIL  $n"; echo "$out" | head -4; fail=$((fail+1)); fi
    continue
  fi
  if [ "$out" = "$(cat "$HERE/$n.expected.txt")" ]; then echo "PASS  $n"; pass=$((pass+1)); else echo "FAIL  $n"; diff <(printf '%s\n' "$out") "$HERE/$n.expected.txt" | head -10; fail=$((fail+1)); fi
done
# the threaded case is repeated: a race that depends on timing shows up as an occasional difference
for i in 1 2 3 4 5; do
  out="$(run_with_timeout "$HERE/threads_disjoint.sleela" 2>&1 | filter)"
  [ "$out" = "$(cat "$HERE/threads_disjoint.expected.txt")" ] || { echo "FAIL  threads_disjoint repeat $i: $out"; fail=$((fail+1)); }
done
echo "runtime pressure: pass=$pass fail=$fail"
[ "$fail" -eq 0 ] && echo "runtime pressure: PASS"
[ "$fail" -eq 0 ]
