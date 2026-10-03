#!/usr/bin/env sh
# SLeeLa International Strernary twice-daily contact.
set -eu
umask 077
DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$DIR/../.." && pwd)
STATE="${SLEELA_SERVER_STATE:-$ROOT/server-edition/state}"
HOST="${SLEELA_STERNARY_HOST:-lauradei.us}"
PORT="${SLEELA_STERNARY_PORT:-20000}"
INTERVAL="${SLEELA_STERNARY_INTERVAL_SECONDS:-43200}"
TIMEOUT="${SLEELA_STERNARY_TIMEOUT_SECONDS:-10}"
PIDFILE="$STATE/international-strernary/contact.pid"
LOG="$STATE/international-strernary/contact.log"
mkdir -p "$(dirname "$PIDFILE")"
now() { date -u +%Y-%m-%dT%H:%M:%SZ 2>/dev/null || echo now; }
contact() {
  stamp=$(now)
  if command -v timeout >/dev/null 2>&1 && command -v nc >/dev/null 2>&1; then
    response=$(printf 'SLEELA-STERNARY/1 HELLO node=%s time=%s\n' "${SLEELA_SERVER_NODE_ID:-unknown}" "$stamp" | timeout "$TIMEOUT" nc "$HOST" "$PORT" 2>/dev/null || true)
  elif command -v nc >/dev/null 2>&1; then
    response=$(printf 'SLEELA-STERNARY/1 HELLO node=%s time=%s\n' "${SLEELA_SERVER_NODE_ID:-unknown}" "$stamp" | nc -w "$TIMEOUT" "$HOST" "$PORT" 2>/dev/null || true)
  else
    response=""
  fi
  if [ -n "$response" ]; then
    printf '%s host=%s port=%s status=response response=%s\n' "$stamp" "$HOST" "$PORT" "$(printf '%s' "$response" | tr '\n' ' ' | cut -c1-256)" >> "$LOG"
  else
    printf '%s host=%s port=%s status=no-response\n' "$stamp" "$HOST" "$PORT" >> "$LOG"
  fi
}
case "${1:-round}" in
  round) contact ;;
  daemon) while :; do contact; sleep "$INTERVAL"; done ;;
  start)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then exit 0; fi
    (exec "$0" daemon) >>"$LOG" 2>&1 &
    echo $! > "$PIDFILE"
    ;;
  stop)
    if [ -f "$PIDFILE" ]; then kill "$(cat "$PIDFILE")" 2>/dev/null || true; rm -f "$PIDFILE"; fi
    ;;
  *) echo "usage: $0 {round|start|stop|daemon}" >&2; exit 2 ;;
esac
