#!/bin/sh
# web-server-technology/native/warm-live-test.sh
#
# Verify the CPU keep-warm idle floor against the live server:
#   - on by default: status reports the band and a live measured value in-band;
#   - --no-warm disables it (admin override).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
BIN="$HERE/build/sleela-web-server"
PORT="${PORT:-8092}"
WORK="$(mktemp -d)"
cleanup() { [ -n "$PID" ] && kill "$PID" 2>/dev/null || true; rm -rf "$WORK"; }
trap cleanup EXIT INT TERM
mkdir -p "$WORK/root"

echo "=== A) default ON: band 11-14, cores clamped, measured in-band ==="
"$BIN" --port "$PORT" --root "$WORK/root" --warm-cores 2 >"$WORK/log" 2>&1 &
PID=$!
sleep 2   # let the controller settle
echo "status: $(curl -s "http://127.0.0.1:$PORT/__sleela/status")"
echo "warm:   $(curl -s "http://127.0.0.1:$PORT/__sleela/warm")"
echo "startup log:"; grep -E 'cpu-warm' "$WORK/log"
kill "$PID" 2>/dev/null || true; wait "$PID" 2>/dev/null || true; PID=

echo ""
echo "=== B) admin disables it: --no-warm ==="
"$BIN" --port "$PORT" --root "$WORK/root" --no-warm >"$WORK/log" 2>&1 &
PID=$!
sleep 1
echo "status: $(curl -s "http://127.0.0.1:$PORT/__sleela/status")"
echo "startup log:"; grep -E 'cpu-warm' "$WORK/log"
