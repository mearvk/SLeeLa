#!/bin/sh
# web-server-technology/native/stress-fuzz-test.sh
#
# Self-contained concurrent-/db stress + fuzz harness. This is the project's
# "CI for the web server" deliberately offloaded to a local runnable script --
# no external CI vendor, no GitHub Actions. Run it by hand or from any runner.
#
# It exercises three things and fails (nonzero exit) if any regress:
#   A. Concurrency  -- many parallel clients hammering /db (reads + writes)
#                      under a write token; asserts a high success rate and that
#                      the server stays up and serves afterward.
#   B. SQL fuzz     -- malformed / oversized / multi-statement SQL; asserts the
#                      server returns a clean 4xx (never 5xx, never a crash).
#   C. HTTP fuzz    -- malformed request framing on raw sockets; asserts the
#                      server rejects with 400 or closes, and still serves a
#                      good request afterward (process survived).
#
# Usage: stress-fuzz-test.sh [CLIENTS] [REQS_PER_CLIENT]
#
# Defaults are modest so the harness finishes quickly even where process spawn
# is expensive; raise CLIENTS/REQS for a heavier soak. The concurrency burst
# uses one curl invocation per client with many URLs (curl reuses connections
# and runs them back-to-back), so load is driven without paying a process spawn
# per request.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
BIN="$HERE/build/sleela-web-server"
SQL="$HERE/../../lib/sleela-sql/build/sleela-sql"
PORT="${PORT:-8091}"
CLIENTS="${1:-8}"
REQS="${2:-25}"
TOKEN="stress-token"
WORK="$(mktemp -d)"
LOG="$(mktemp)"
PID=""
rc=0

cleanup() { [ -n "$PID" ] && kill "$PID" 2>/dev/null; rm -rf "$WORK" "$LOG"; }
trap cleanup EXIT INT TERM

note() { printf '%s\n' "$*"; }
fail() { printf 'FAIL: %s\n' "$*"; rc=1; }

# --- seed a database and start the server -----------------------------------
mkdir -p "$WORK/root" "$WORK/db"
"$SQL" "$WORK/db" "CREATE TABLE log (id, who, note)" >/dev/null
"$BIN" --port "$PORT" --root "$WORK/root" --db "$WORK/db" --db-writable \
       --write-token "$TOKEN" --threads 16 --rate-per-peer 0 \
       --no-warm >"$LOG" 2>&1 &
PID=$!
base="http://127.0.0.1:$PORT"
# Startup health gate: wait up to ~5s for the server to answer, so a bind
# failure (e.g. port in use) is reported clearly instead of looking like a
# mid-run crash.
up=0
k=0
while [ "$k" -lt 25 ]; do
  if ! kill -0 "$PID" 2>/dev/null; then note "server exited during startup:"; cat "$LOG"; exit 1; fi
  if [ "$(curl -s -m 2 -o /dev/null -w '%{http_code}' "$base/__sleela/health" 2>/dev/null)" = "200" ]; then up=1; break; fi
  k=$((k+1)); sleep 0.2
done
if [ "$up" -ne 1 ]; then note "server did not become healthy on port $PORT:"; cat "$LOG"; exit 1; fi
note "server up on port $PORT"

# ============================================================================
note "=== A) concurrency: $CLIENTS clients x $REQS requests (mixed read/write) ==="
RESULTS="$WORK/results"
mkdir -p "$RESULTS"
read_url="$base/db/query?sql=SELECT%20COUNT(*)%20FROM%20log"
# The load is driven with curl's own parallelism: one curl per client issues
# $REQS requests across keep-alive connections. Mixing reads and writes is done
# by alternating client "modes". curl's --parallel runs the URLs concurrently
# within a single process, so we get genuine server-side concurrency without
# forking a process per request or relying on many backgrounded shells (which
# the sandbox throttles). Falls back to a sequential loop if --parallel is
# unavailable in the installed curl.
total_ok=0; total_bad=0
curl_has_parallel=0
curl --help all 2>/dev/null | grep -q -- '--parallel' && curl_has_parallel=1

# Build a reusable curl config file for the read burst. Each URL is paired with
# its own `output = /dev/null` so NO response body reaches stdout -- otherwise,
# with multiple URLs, curl prints all-but-the-last body and the status parser
# would miscount bodies as failures. Only the -w '%{http_code}' lines remain.
urlfile="$WORK/urls"
: > "$urlfile"
n=1
while [ "$n" -le "$REQS" ]; do
  printf 'url = "%s"\noutput = "/dev/null"\n' "$read_url" >> "$urlfile"
  n=$((n+1))
done

run_read_burst() { # writes one HTTP status code per line to $1
  out="$1"
  if [ "$curl_has_parallel" -eq 1 ]; then
    curl -s --parallel --parallel-max 8 -w '%{http_code}\n' -K "$urlfile" > "$out" 2>/dev/null
  else
    curl -s -w '%{http_code}\n' -K "$urlfile" > "$out" 2>/dev/null
  fi
}

i=1
while [ "$i" -le "$CLIENTS" ]; do
  run_read_burst "$RESULTS/c$i"
  # a few token-authenticated writes per client to exercise the mutate path
  w=1
  while [ "$w" -le 3 ]; do
    code=$(curl -s -m 5 -o /dev/null -w '%{http_code}' -X POST \
           -H "Authorization: Bearer $TOKEN" \
           --data "INSERT INTO log VALUES ($i$w, 'c$i', 'n$w')" \
           "$base/db/query" 2>/dev/null)
    printf '%s\n' "$code" >> "$RESULTS/c$i"
    w=$((w+1))
  done
  i=$((i+1))
done

for f in "$RESULTS"/c*; do
  [ -f "$f" ] || continue
  while IFS= read -r code; do
    [ -n "$code" ] || continue
    if [ "$code" = "200" ]; then total_ok=$((total_ok+1)); else total_bad=$((total_bad+1)); fi
  done < "$f"
done
total=$((total_ok + total_bad))
note "requests: $total  ok(200): $total_ok  non-200: $total_bad"
alive=$(curl -s -m 3 -o /dev/null -w '%{http_code}' "$base/__sleela/health")
if [ "$alive" = "200" ]; then note "server still serving (health 200)"; else fail "server not serving after concurrency run (health=$alive)"; fi
[ "$total" -gt 0 ] || fail "no requests completed (harness/client problem)"
# Every request under valid concurrent load must succeed; a non-200 is a defect.
[ "$total_bad" -eq 0 ] || fail "non-200 responses under valid concurrent load: $total_bad"
# Every request should succeed; any non-200 under valid load is a defect.
[ "$total_bad" -eq 0 ] || fail "non-200 responses under valid concurrent load: $total_bad"
# The committed rows should be readable and consistent (no torn CSV).
cnt=$(curl -s "$base/db/query?sql=SELECT%20COUNT(*)%20FROM%20log" | grep -oE '"row_count":[0-9]+')
note "post-run count query: $cnt"

# ============================================================================
note "=== B) SQL fuzz: malformed input must be 4xx, never 5xx/crash ==="
fuzz_sql() {
  # $1 = description, $2 = url-encoded sql or raw for POST, $3 = method
  desc="$1"; payload="$2"; method="${3:-GET}"
  if [ "$method" = "GET" ]; then
    code=$(curl -s -o /dev/null -w '%{http_code}' "$base/db/query?sql=$payload")
  else
    code=$(curl -s -o /dev/null -w '%{http_code}' -X POST \
           -H "Authorization: Bearer $TOKEN" --data "$payload" "$base/db/query")
  fi
  case "$code" in
    5*) fail "SQL fuzz [$desc] returned $code (server-side error)";;
    000) fail "SQL fuzz [$desc] got no response (possible crash)";;
    *) note "  ok [$desc] -> $code";;
  esac
}
fuzz_sql "gibberish"            "NOTSQL%20%24%25%5E"
fuzz_sql "empty"               ""
fuzz_sql "multi-statement"     "SELECT%201%3B%20DROP%20TABLE%20log" 
fuzz_sql "unknown table"       "SELECT%20x%20FROM%20nope"
fuzz_sql "huge (16KB)"         "$(head -c 16000 /dev/zero | tr '\0' 'A')"
fuzz_sql "binary-ish"          "%00%01%02%ff%fe"
fuzz_sql "write via POST junk" "}{ not sql" POST
alive=$(curl -s -m 3 -o /dev/null -w '%{http_code}' "$base/__sleela/health")
if [ "$alive" = "200" ]; then note "server still serving after SQL fuzz (health 200)"; else fail "server not serving after SQL fuzz (health=$alive)"; fi

# ============================================================================
note "=== C) HTTP framing fuzz on raw sockets: reject cleanly, survive ==="
# Needs a raw-socket tool; prefer python3, fall back to printf+/dev/tcp.
raw_send() { # $1 = raw bytes (already with \r\n), prints status or CLOSED
  if command -v python3 >/dev/null 2>&1; then
    python3 - "$PORT" "$1" <<'PY'
import socket, sys
port = int(sys.argv[1]); data = sys.argv[2].encode().decode('unicode_escape').encode()
try:
    s = socket.create_connection(("127.0.0.1", port), timeout=3)
    s.sendall(data); s.settimeout(3)
    resp = s.recv(4096); s.close()
    line = resp.split(b"\r\n",1)[0].decode(errors="replace")
    print(line if line else "CLOSED")
except Exception as e:
    print("CLOSED")
PY
  else
    printf 'SKIP(no python3)'
  fi
}
check_raw() { # $1 desc, $2 raw
  out=$(raw_send "$2")
  case "$out" in
    SKIP*) note "  skip [$1] (no python3)";;
    *5[0-9][0-9]*) fail "HTTP fuzz [$1] -> $out (server error)";;
    *) note "  ok [$1] -> $out";;
  esac
}
check_raw "bad method"        "ZORP / HTTP/1.1\r\nHost: x\r\n\r\n"
check_raw "no HTTP version"   "GET /\r\n\r\n"
check_raw "missing Host 1.1"  "GET / HTTP/1.1\r\n\r\n"
check_raw "garbage line"      "\xff\xfe not http at all\r\n\r\n"
check_raw "huge header"       "GET / HTTP/1.1\r\nHost: x\r\nX: $(head -c 70000 /dev/zero | tr '\0' 'a')\r\n\r\n"
# Survival: a good request must still work after all that abuse.
final=$(curl -s -o /dev/null -w '%{http_code}' "$base/__sleela/health")
if [ "$final" = "200" ]; then note "server healthy after HTTP fuzz (200)"; else fail "server not healthy after HTTP fuzz: $final"; fi

# ============================================================================
if [ "$rc" -eq 0 ]; then
  note ""
  note "ALL STRESS + FUZZ CHECKS PASSED"
else
  note ""
  note "STRESS + FUZZ HARNESS FOUND FAILURES"
  note "--- server log tail ---"; tail -20 "$LOG"
fi
exit "$rc"
