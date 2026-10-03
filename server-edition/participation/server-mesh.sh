#!/usr/bin/env sh
# SLeeLa Server Participation — daily/few-times-per-day fast regroup.
set -eu
umask 077
DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$DIR/../.." && pwd)
STATE="${SLEELA_SERVER_STATE:-$ROOT/server-edition/state}"
PEERS="${SLEELA_MESH_PEERS_FILE:-$DIR/known-servers.conf}"
PORT="${SLEELA_MESH_PORT:-22221}"
INTERVAL="${SLEELA_MESH_INTERVAL_SECONDS:-28800}"
TIMEOUT="${SLEELA_MESH_TIMEOUT_SECONDS:-5}"
MAX_PEERS="${SLEELA_MESH_MAX_PEERS:-64}"
PIDFILE="$STATE/participation/mesh.pid"
LOG="$STATE/participation/mesh.log"
LOCK="$STATE/participation/.mesh.lock"
mkdir -p "$STATE/participation"
now() { date -u +%Y-%m-%dT%H:%M:%SZ 2>/dev/null || echo now; }
log() { printf '%s %s\n' "$(now)" "$*" >> "$LOG"; }
load_peers() {
  [ -f "$PEERS" ] || return 0
  awk 'BEGIN{FS="[[:space:]]+"} /^[[:space:]]*#/ {next} NF>=2 {print $1" "$2}' "$PEERS" | head -n "$MAX_PEERS"
}
send_one() {
  host=$1; port=$2; message=$3
  response="$STATE/participation/.response.$$.$RANDOM"
  if command -v timeout >/dev/null 2>&1 && command -v nc >/dev/null 2>&1; then
    if printf '%s\n' "$message" | timeout "$TIMEOUT" nc "$host" "$port" >"$response" 2>/dev/null; then
      log "peer=$host:$port status=response response=$(tr '\n' ' ' < "$response" | cut -c1-256)"
      rm -f "$response"; return 0
    fi
  elif command -v nc >/dev/null 2>&1; then
    if printf '%s\n' "$message" | nc -w "$TIMEOUT" "$host" "$port" >"$response" 2>/dev/null; then
      log "peer=$host:$port status=response response=$(tr '\n' ' ' < "$response" | cut -c1-256)"
      rm -f "$response"; return 0
    fi
  fi
  log "peer=$host:$port status=no-response"
  rm -f "$response"; return 1
}
round() {
  [ -f "$PEERS" ] || { log "round=skipped reason=no-known-servers-file"; return 0; }
  mkdir "$LOCK" 2>/dev/null || { log "round=skipped reason=already-running"; return 0; }
  trap 'rmdir "$LOCK" 2>/dev/null || true' EXIT HUP INT TERM
  stamp=$(now)
  node="${SLEELA_SERVER_NODE_ID:-$(hostname 2>/dev/null || echo unknown)}"
  message="SLEELA-MESH/1 HELLO node=$node time=$stamp port=$PORT"
  log "round=start peers=$(load_peers | wc -l | tr -d ' ')"
  pids=""; count=0
  while read -r host peer_port; do
    [ -n "$host" ] || continue
    [ "$count" -lt "$MAX_PEERS" ] || break
    peer_port="${peer_port:-$PORT}"
    send_one "$host" "$peer_port" "$message" &
    pids="$pids $!"
    count=$((count + 1))
  done <<EOF
$(load_peers)
EOF
  rc=0
  for pid in $pids; do wait "$pid" || rc=1; done
  log "round=regroup complete peers=$count status=$rc"
  trap - EXIT HUP INT TERM
  rmdir "$LOCK" 2>/dev/null || true
}
case "${1:-round}" in
  round) round ;;
  daemon) log "daemon=start interval=$INTERVAL"; while :; do round; sleep "$INTERVAL"; done ;;
  start)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then echo "SLeeLa participation already running: $(cat "$PIDFILE")"; exit 0; fi
    ( exec "$0" daemon ) >>"$LOG" 2>&1 &
    echo $! > "$PIDFILE"; echo "SLeeLa participation started: $(cat "$PIDFILE")"
    ;;
  stop)
    if [ -f "$PIDFILE" ]; then kill "$(cat "$PIDFILE")" 2>/dev/null || true; rm -f "$PIDFILE"; fi
    echo "SLeeLa participation stopped"
    ;;
  *) echo "usage: $0 {round|start|stop|daemon}" >&2; exit 2 ;;
esac
