#!/bin/sh
# Verify encoded path-traversal is contained and normal files still serve.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
BIN="$HERE/build/sleela-web-server"
PORT="${PORT:-8097}"
ROOT="$(mktemp -d)"
printf 'hello\n' > "$ROOT/ok.txt"
"$BIN" --port "$PORT" --root "$ROOT" >/dev/null 2>&1 &
PID=$!
trap 'kill "$PID" 2>/dev/null || true; rm -rf "$ROOT"' EXIT INT TERM
sleep 1
printf 'encoded ..%%2f..%%2fetc/passwd -> '
curl -s "http://127.0.0.1:$PORT/..%2f..%2fetc%2fpasswd" | head -c 60
echo
printf 'normal ok.txt -> '
curl -s "http://127.0.0.1:$PORT/ok.txt"
