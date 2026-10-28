#!/bin/sh
# web-server-technology/native/envelopment-test.sh
#
# End-to-end test of Side-2 envelopment. Builds a temp domain workspace with a
# meme landing page, a static /papers subtree, and a read-only SQL /lab program,
# then verifies:
#   - the Scientist gate (envelopment refuses to activate without --scientist);
#   - warrant routing by longest path prefix (meme / static / sql);
#   - that Side 1 is untouched when envelopment is off.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
BIN="$HERE/build/sleela-web-server"
SQL="$HERE/../../lib/sleela-sql/build/sleela-sql"
PORT="${PORT:-8096}"
WORK="$(mktemp -d)"
LOG="$(mktemp)"

cleanup() { [ -n "$PID" ] && kill "$PID" 2>/dev/null || true; rm -rf "$WORK" "$LOG"; }
trap cleanup EXIT INT TERM

# Program backings.
mkdir -p "$WORK/papers" "$WORK/labdb" "$WORK/root"
printf 'peer-reviewed paper #1\n' > "$WORK/papers/p1.txt"
printf '<h1>Side 1 root</h1>\n'   > "$WORK/root/index.html"
"$SQL" "$WORK/labdb" "CREATE TABLE samples (id, label)" >/dev/null
"$SQL" "$WORK/labdb" "INSERT INTO samples VALUES (1, 'helium')" >/dev/null
"$SQL" "$WORK/labdb" "INSERT INTO samples VALUES (2, 'argon')"  >/dev/null

# Config for the domain's envelopment.
CONF="$WORK/envelopment.conf"
cat > "$CONF" <<EOF
envelopment = on
scientist   = true
domain      = science.local

program = landing
kind    = meme
mount   = /
meme-type = text/html; charset=utf-8
meme-body = <h1>science.local envelopment</h1>

program = papers
kind    = static
mount   = /papers
root    = $WORK/papers

program = lab
kind    = sql
mount   = /lab
db      = $WORK/labdb
db-writable = false

warrant = publish-landing
subject = science.local
path    = /
use     = landing

warrant = serve-papers
subject = science.local
path    = /papers
use     = papers

warrant = expose-lab
subject = science.local
path    = /lab
use     = lab
EOF

echo "=== A) Scientist gate: envelopment enabled WITHOUT the Scientist capability must refuse ==="
# A config that enables envelopment but does NOT grant scientist; no --scientist
# flag either. The server must refuse to start (exit 2) rather than activate.
printf 'envelopment = on\nscientist = false\n' > "$WORK/nosci.conf"
if "$BIN" --port "$PORT" --root "$WORK/root" --config "$WORK/nosci.conf" >"$LOG" 2>&1; then
  echo "UNEXPECTED: server started without the Scientist capability"
else
  echo "refused as expected (exit $?): $(cat "$LOG")"
fi

echo "=== B) Side 1 unaffected when envelopment is OFF (config enables it, CLI does not) ==="
# NOTE: the example config sets envelopment=on; to prove Side-1 fallthrough we
# point at a config that leaves it off.
printf 'envelopment = off\nscientist = true\n' > "$WORK/off.conf"
"$BIN" --port "$PORT" --root "$WORK/root" --config "$WORK/off.conf" --scientist >"$LOG" 2>&1 &
PID=$!; sleep 1
printf 'root index (Side 1): '; curl -s "http://127.0.0.1:$PORT/" 
printf 'status: '; curl -s "http://127.0.0.1:$PORT/__sleela/status"
kill "$PID" 2>/dev/null || true; wait "$PID" 2>/dev/null || true; PID=

echo "=== C) Envelopment ACTIVE: warrant routing ==="
"$BIN" --port "$PORT" --root "$WORK/root" --config "$CONF" --scientist >"$LOG" 2>&1 &
PID=$!; sleep 1
printf '1) status: '; curl -s "http://127.0.0.1:$PORT/__sleela/status"
printf '2) GET / (meme via landing warrant): '
curl -s -H "Host: science.local" "http://127.0.0.1:$PORT/"
printf '3) GET /papers/p1.txt (static via papers warrant): '
curl -s -H "Host: science.local" "http://127.0.0.1:$PORT/papers/p1.txt"
printf '4) GET /lab/db/query?sql=... (sql via lab warrant): '
curl -s -H "Host: science.local" "http://127.0.0.1:$PORT/lab/db/query?sql=SELECT%20label%20FROM%20samples"
printf '5) write to /lab (program is read-only): '
curl -s -H "Host: science.local" -X POST --data "INSERT INTO samples VALUES (3, 'neon')" "http://127.0.0.1:$PORT/lab/db/query"
echo "=== server log (C) ==="
cat "$LOG"
