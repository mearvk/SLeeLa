#!/usr/bin/env sh
# SLeeLa Server Participation response listener.
set -eu
umask 077
DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$DIR/../.." && pwd)
STATE="${SLEELA_SERVER_STATE:-$ROOT/server-edition/state}"
PORT="${SLEELA_MESH_PORT:-22221}"
PIDFILE="$STATE/participation/listener.pid"
LOG="$STATE/participation/mesh-listener.log"
mkdir -p "$STATE/participation"
now() { date -u +%Y-%m-%dT%H:%M:%SZ 2>/dev/null || echo now; }
run_listener() {
  while :; do
    if command -v ncat >/dev/null 2>&1; then
      printf 'SLEELA-MESH/1 ACK time=%s\n' "$(now)" | ncat --listen --source-port "$PORT" --send-only 2>/dev/null || true
    elif command -v nc >/dev/null 2>&1; then
      if nc -h 2>&1 | grep -q -- '-k'; then
        printf 'SLEELA-MESH/1 ACK time=%s\n' "$(now)" | nc -lk "$PORT" 2>/dev/null || true
      else
        printf 'SLEELA-MESH/1 ACK time=%s\n' "$(now)" | nc -l "$PORT" 2>/dev/null || printf 'SLEELA-MESH/1 ACK time=%s\n' "$(now)" | nc -l -p "$PORT" 2>/dev/null || true
      fi
    else
      echo "$(now) status=unavailable reason=no-nc" >> "$LOG"
      sleep 60
    fi
  done
}
case "${1:-start}" in
  start)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then exit 0; fi
    (run_listener) >>"$LOG" 2>&1 &
    echo $! > "$PIDFILE"
    echo "SLeeLa participation listener started: $(cat "$PIDFILE")"
    ;;
  stop)
    if [ -f "$PIDFILE" ]; then kill "$(cat "$PIDFILE")" 2>/dev/null || true; rm -f "$PIDFILE"; fi
    ;;
  *) echo "usage: $0 {start|stop}" >&2; exit 2 ;;
esac
