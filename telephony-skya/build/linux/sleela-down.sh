#!/bin/sh
# sleela-down.sh -- stop the SLeeLa side started by sleela-up.sh.
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
PIDFILE="$ROOT/build/linux/.skya-sleela.pids"
[ -f "$PIDFILE" ] || { echo "sleela-down: nothing to stop ($PIDFILE missing)"; exit 0; }
while IFS= read -r pid; do
    [ -n "$pid" ] || continue
    kill "$pid" 2>/dev/null || true
done < "$PIDFILE"
rm -f "$PIDFILE"
echo "Skya SLeeLa side stopped."
