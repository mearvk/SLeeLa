#!/usr/bin/env sh
# SLeeLa server traffic logger.
# Records connection/packet metadata for configured probe and service ports.
# Payload capture is best-effort and requires tcpdump privileges/capabilities.
set -eu
STATE=${SLEELA_SERVER_STATE:-server-edition/state}
LOG_DIR="${SLEELA_TRAFFIC_LOG_DIR:-$STATE/traffic}"
PIDFILE="$LOG_DIR/traffic.pid"
LOGFILE="$LOG_DIR/traffic.log"
CONSUMER_PIDFILE="$LOG_DIR/consumer.pid"
PORTS="${SLEELA_TRAFFIC_PORTS:-2222,22220,19866,20000}"
mkdir -p "$LOG_DIR"
ports_expr=$(printf '%s' "$PORTS" | awk -F, '{
  for(i=1;i<=NF;i++){ gsub(/[^0-9]/,"",$i); if($i!="") { if(n++) printf " or "; printf "tcp port %s or udp port %s",$i,$i } }
}')
case "${1:-}" in
  start)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then exit 0; fi
    if command -v tcpdump >/dev/null 2>&1; then
      # -nn preserves numeric IP/port data; -s 0 retains full packets; -X records payload bytes.
      nohup tcpdump -nn -tttt -vvv -i any -s 0 -X "$ports_expr" >>"$LOGFILE" 2>&1 &
      echo $! >"$PIDFILE"
      printf '%s traffic_logger=start ports=%s\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ 2>/dev/null || echo now)" "$PORTS" >>"$LOGFILE"
      CONSUMER="$STATE/../port-awareness/log-consumer.sh"
      [ -f "$CONSUMER" ] || CONSUMER="server-edition/port-awareness/log-consumer.sh"
      if [ -f "$CONSUMER" ] && ! { [ -f "$CONSUMER_PIDFILE" ] && kill -0 "$(cat "$CONSUMER_PIDFILE")" 2>/dev/null; }; then
        ( while :; do SLEELA_SERVER_STATE="$STATE" SLEELA_TRAFFIC_LOG_DIR="$LOG_DIR" sh "$CONSUMER" || true; sleep 60; done ) >/dev/null 2>&1 &
        echo $! >"$CONSUMER_PIDFILE"
      fi
    else
      printf '%s traffic_logger=unavailable reason=tcpdump-not-installed ports=%s\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ 2>/dev/null || echo now)" "$PORTS" >>"$LOGFILE"
    fi
    ;;
  stop)
    if [ -f "$CONSUMER_PIDFILE" ]; then kill "$(cat "$CONSUMER_PIDFILE")" 2>/dev/null || true; rm -f "$CONSUMER_PIDFILE"; fi
    if [ -f "$PIDFILE" ]; then
      kill "$(cat "$PIDFILE")" 2>/dev/null || true
      rm -f "$PIDFILE"
    fi
    printf '%s traffic_logger=stop ports=%s\n' "$(date -u +%Y-%m-%dT%H:%M:%SZ 2>/dev/null || echo now)" "$PORTS" >>"$LOGFILE"
    ;;
  *)
    echo "usage: $0 start|stop" >&2
    exit 2
    ;;
esac
