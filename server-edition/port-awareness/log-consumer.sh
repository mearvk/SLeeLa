#!/usr/bin/env sh
# SLeeLa privacy-preserving traffic-log consumer.
# Produces dated message.digest files and consumes the source log daily or at 50 MiB.
set -eu
STATE=${SLEELA_SERVER_STATE:-server-edition/state}
TRAFFIC_DIR=${SLEELA_TRAFFIC_LOG_DIR:-$STATE/traffic}
OUT_DIR=${SLEELA_MESSAGE_DIGEST_DIR:-$STATE/message-digest}
LOGFILE=$TRAFFIC_DIR/traffic.log
mkdir -p "$TRAFFIC_DIR" "$OUT_DIR"
MAX_BYTES=${SLEELA_LOG_MAX_BYTES:-52428800}
STAMP=$(date -u +%Y-%m-%dT%H-%M-%SZ 2>/dev/null || echo now)
DAY=$(date -u +%Y-%m-%d 2>/dev/null || echo unknown-date)
HOUR=$(date -u +%H-%M-%S 2>/dev/null || echo unknown-time)
TARGET="$OUT_DIR/$DAY/$HOUR/message.digest"
should_consume=false
if [ -f "$LOGFILE" ]; then
  size=$(wc -c < "$LOGFILE" | tr -d ' ')
  [ "${size:-0}" -ge "$MAX_BYTES" ] && should_consume=true
  if [ -n "${SLEELA_LAST_LOG_DAY:-}" ] && [ "${SLEELA_LAST_LOG_DAY}" != "$DAY" ]; then should_consume=true; fi
  [ "${SLEELA_FORCE_LOG_CONSUME:-false}" = true ] && should_consume=true
fi
[ "$should_consume" = true ] || exit 0
mkdir -p "$(dirname "$TARGET")"
{
  printf 'schema=sleela.message.digest.v1 timestamp=%s\n' "$STAMP"
  printf 'privacy=ip-redacted national-data-redacted contact-data-redacted payload-redacted\n'
  printf 'source_bytes=%s\n' "${size:-0}"
  if [ -s "$LOGFILE" ]; then
    # Keep protocol/port/event structure while removing addresses, identity/contact fields,
    # and packet payload. This digest intentionally cannot reconstruct a remote identity.
    sed -E \
      -e 's/([0-9A-Fa-f]{0,4}:){2,}[0-9A-Fa-f:]+/[IP-REDACTED]/g' \
      -e 's/[0-9]{1,3}(\.[0-9]{1,3}){3}/[IP-REDACTED]/g' \
      -e 's/([[:space:]]|^)(National|Citizen|Bank|Banker|Cromsmrus|Contact|Email|Phone|Address|Identity)[_-]?[A-Za-z0-9.-]*=[^[:space:]]+/\1[DATA-REDACTED]/gI' \
      -e 's/([[:space:]]|^)([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}([[:space:]]|$)/\1[MAC-REDACTED]\3/g' \
      -e 's/0x[0-9A-Fa-f]+/[PAYLOAD-REDACTED]/g' \
      -e 's/[[:space:]]+([0-9A-Fa-f]{2}[[:space:]]+){8,}.*/ [PAYLOAD-REDACTED]/g' \
      "$LOGFILE" | awk '
        /IP-REDACTED|DATA-REDACTED|MAC-REDACTED|PAYLOAD-REDACTED|traffic_logger=/ {
          gsub(/[[:space:]]+/, " ");
          print
        }
      '
  fi
} > "$TARGET"
# Atomic consume: the raw packet log is no longer retained after successful digest creation.
: > "$LOGFILE"
printf '%s consumed source=traffic.log target=%s\n' "$STAMP" "$TARGET" >> "$TRAFFIC_DIR/consumer.log"
printf '%s\n' "$DAY" > "$TRAFFIC_DIR/last-consumed-day"
