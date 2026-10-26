#!/bin/sh
# web-server-technology/native/smoke-test.sh
# End-to-end smoke test for the dynamic SQL-backed server.
# Seeds a database, starts the server, exercises static + dynamic endpoints,
# verifies the read-only policy, then shuts the server down.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
BIN="$HERE/build/sleela-web-server"
SQL="$HERE/../../lib/sleela-sql/build/sleela-sql"
PORT="${PORT:-8099}"
DBDIR="$(mktemp -d)"
ROOT="$(mktemp -d)"
LOG="$(mktemp)"

cleanup() {
  [ -n "$PID" ] && kill "$PID" 2>/dev/null || true
  rm -rf "$DBDIR" "$ROOT" "$LOG"
}
trap cleanup EXIT INT TERM

"$SQL" "$DBDIR" "CREATE TABLE cities (id, name, country)" >/dev/null
"$SQL" "$DBDIR" "INSERT INTO cities VALUES (1, 'Tokyo', 'Japan')" >/dev/null
"$SQL" "$DBDIR" "INSERT INTO cities VALUES (2, 'Lagos', 'Nigeria')" >/dev/null
"$SQL" "$DBDIR" "INSERT INTO cities VALUES (3, 'Lima', 'Peru')" >/dev/null
printf '<h1>SLeeLa WST</h1>\n' > "$ROOT/index.html"

MODE="${1:-ro}"
if [ "$MODE" = "rw" ]; then
  "$BIN" --port "$PORT" --root "$ROOT" --db "$DBDIR" --db-writable >"$LOG" 2>&1 &
else
  "$BIN" --port "$PORT" --root "$ROOT" --db "$DBDIR" >"$LOG" 2>&1 &
fi
PID=$!
sleep 1

echo "=== 1) status ==="
curl -s "http://127.0.0.1:$PORT/__sleela/status"
echo "=== 2) db health ==="
curl -s "http://127.0.0.1:$PORT/db/health"
echo "=== 3) static index ==="
curl -s "http://127.0.0.1:$PORT/index.html"
echo "=== 4) dynamic SELECT (GET) ==="
curl -s "http://127.0.0.1:$PORT/db/query?sql=SELECT%20id,name,country%20FROM%20cities"
echo "=== 5) WHERE filter ==="
curl -s "http://127.0.0.1:$PORT/db/query?sql=SELECT%20name%20FROM%20cities%20WHERE%20country%20=%20'Peru'"
echo "=== 6) fluent SLeeLaSQL (POST) ==="
curl -s -X POST --data "from('cities').select(name).where(id == '1')" "http://127.0.0.1:$PORT/db/query"
echo "=== 7) write via POST (policy: $MODE) ==="
curl -s -X POST --data "INSERT INTO cities VALUES (4, 'Oslo', 'Norway')" "http://127.0.0.1:$PORT/db/query"
echo "=== 8) IPv6 loopback (dual-stack) ==="
curl -s "http://[::1]:$PORT/db/health" || echo "(ipv6 loopback unavailable in this env)"
echo "=== 9) traversal blocked ==="
curl -s "http://127.0.0.1:$PORT/../../etc/passwd" | head -1
echo "=== server log ==="
cat "$LOG"
