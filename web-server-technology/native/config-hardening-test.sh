#!/bin/sh
# Verify hardening settings supplied via the CONFIG FILE take effect
# (rate-per-peer and write-token), demonstrating the file control surface.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
BIN="$HERE/build/sleela-web-server"
PORT="${PORT:-8093}"
WORK="$(mktemp -d)"
cleanup() { [ -n "$PID" ] && kill "$PID" 2>/dev/null || true; rm -rf "$WORK"; }
trap cleanup EXIT INT TERM

mkdir -p "$WORK/root" "$WORK/db"
printf 'ok\n' > "$WORK/root/p.txt"
cat > "$WORK/h.conf" <<EOF
# hardening entirely from the config file
rate-per-peer = 20
rate-window-ms = 1000
write-token = fromfile
EOF

"$BIN" --port "$PORT" --root "$WORK/root" --db "$WORK/db" --db-writable --config "$WORK/h.conf" >"$WORK/log" 2>&1 &
PID=$!; sleep 1
echo "status: $(curl -s "http://127.0.0.1:$PORT/__sleela/status")"
for n in 1 2 3; do
  echo "req $n -> $(curl -s -o /dev/null -w '%{http_code}' "http://127.0.0.1:$PORT/p.txt")"
done
echo "write without token -> $(curl -s -o /dev/null -w '%{http_code}' -X POST --data 'CREATE TABLE z (a)' "http://127.0.0.1:$PORT/db/query")"
echo "write with file token -> $(curl -s -X POST -H 'Authorization: Bearer fromfile' --data 'CREATE TABLE z (a)' "http://127.0.0.1:$PORT/db/query")"
