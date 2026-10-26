#!/bin/sh
# web-server-technology/native/hardening-live-test.sh
#
# End-to-end test of the Section-3 hardening features against the live server:
#   - per-peer rate limiting returns 503 past the cap, recovers after the window;
#   - write-token: a mutating /db statement is 401 without the bearer token and
#     succeeds with it;
#   - status reports the hardening flags.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
BIN="$HERE/build/sleela-web-server"
SQL="$HERE/../../lib/sleela-sql/build/sleela-sql"
PORT="${PORT:-8094}"
WORK="$(mktemp -d)"
LOG="$(mktemp)"
cleanup() { [ -n "$PID" ] && kill "$PID" 2>/dev/null || true; rm -rf "$WORK" "$LOG"; }
trap cleanup EXIT INT TERM

mkdir -p "$WORK/root" "$WORK/db"
printf 'ok\n' > "$WORK/root/ping.txt"
"$SQL" "$WORK/db" "CREATE TABLE t (id, v)" >/dev/null
"$SQL" "$WORK/db" "INSERT INTO t VALUES (1, 'a')" >/dev/null

echo "=== A) rate limit: 3 req / 1000ms per peer, 4th is 503 ==="
"$BIN" --port "$PORT" --root "$WORK/root" --db "$WORK/db" \
       --rate-per-peer 3 --rate-window-ms 1000 >"$LOG" 2>&1 &
PID=$!; sleep 1
for n in 1 2 3 4; do
  code=$(curl -s -o /dev/null -w '%{http_code}' "http://127.0.0.1:$PORT/ping.txt")
  echo "request $n -> HTTP $code"
done
echo "waiting out the window (1.1s)..."; sleep 1.1
code=$(curl -s -o /dev/null -w '%{http_code}' "http://127.0.0.1:$PORT/ping.txt")
echo "after window -> HTTP $code"
echo "status: $(curl -s "http://127.0.0.1:$PORT/__sleela/status")"
kill "$PID" 2>/dev/null || true; wait "$PID" 2>/dev/null || true; PID=

echo ""
echo "=== B) write-token: mutation needs Authorization: Bearer <token> ==="
"$BIN" --port "$PORT" --root "$WORK/root" --db "$WORK/db" --db-writable \
       --write-token "topsecret" >"$LOG" 2>&1 &
PID=$!; sleep 1
echo -n "1) read without token (allowed): "
curl -s "http://127.0.0.1:$PORT/db/query?sql=SELECT%20v%20FROM%20t"
echo -n "2) write WITHOUT token (401): "
curl -s -o /dev/null -w 'HTTP %{http_code}\n' -X POST --data "INSERT INTO t VALUES (2, 'b')" "http://127.0.0.1:$PORT/db/query"
echo -n "3) write with WRONG token (401): "
curl -s -o /dev/null -w 'HTTP %{http_code}\n' -X POST -H "Authorization: Bearer nope" --data "INSERT INTO t VALUES (2, 'b')" "http://127.0.0.1:$PORT/db/query"
echo -n "4) write with correct token (ok): "
curl -s -X POST -H "Authorization: Bearer topsecret" --data "INSERT INTO t VALUES (2, 'b')" "http://127.0.0.1:$PORT/db/query"
echo -n "5) confirm the row landed: "
curl -s "http://127.0.0.1:$PORT/db/query?sql=SELECT%20COUNT(*)%20FROM%20t"
echo "=== server log (B) ==="
grep -E 'write-unauthorized|write-denied|db-query ok|status=401' "$LOG" | tail -6
kill "$PID" 2>/dev/null || true; wait "$PID" 2>/dev/null || true; PID=

echo ""
echo "=== C) trusted-proxy: client IP from a TLS terminator, with anti-spoofing ==="
# Trust 127.0.0.1 as the terminator. Rate-limit per peer = 1 so we can observe
# which client identity the limiter is keyed on.
"$BIN" --port "$PORT" --root "$WORK/root" \
       --trusted-proxy 127.0.0.1 --rate-per-peer 1 --rate-window-ms 5000 >"$LOG" 2>&1 &
PID=$!; sleep 1
# First request as forwarded client 9.9.9.9 (allowed; keys the limiter on 9.9.9.9).
echo "req A (XFF 9.9.9.9) -> $(curl -s -o /dev/null -w '%{http_code}' -H 'X-Forwarded-For: 9.9.9.9' "http://127.0.0.1:$PORT/ping.txt")"
# Second request as a DIFFERENT forwarded client 8.8.8.8 is a different identity
# -> allowed (proves the limiter keys on the forwarded client, not the peer).
echo "req B (XFF 8.8.8.8) -> $(curl -s -o /dev/null -w '%{http_code}' -H 'X-Forwarded-For: 8.8.8.8' "http://127.0.0.1:$PORT/ping.txt")"
# Repeat client 9.9.9.9 -> now over its per-peer cap -> 503.
echo "req C (XFF 9.9.9.9 again) -> $(curl -s -o /dev/null -w '%{http_code}' -H 'X-Forwarded-For: 9.9.9.9' "http://127.0.0.1:$PORT/ping.txt")"
echo "client identities seen in log:"
grep -oE 'client=[0-9.]+' "$LOG" | sort | uniq -c
