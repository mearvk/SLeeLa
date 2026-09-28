#!/bin/sh
set -eu
URL="${HTTP4_UPDATE_URL:-https://www.iana.org/}"
INTERVAL_SECONDS="${HTTP4_UPDATE_INTERVAL_SECONDS:-7.5}"
TIMEOUT_SECONDS="${HTTP4_UPDATE_TIMEOUT_SECONDS:-5}"
LOG="${HTTP4_UPDATE_LOG:-http4-update.log}"

fetch_once() {
    if command -v curl >/dev/null 2>&1; then
        curl --fail --silent --show-error --max-time "$TIMEOUT_SECONDS" --output /dev/null "$URL"
    elif command -v wget >/dev/null 2>&1; then
        wget --quiet --timeout="$TIMEOUT_SECONDS" --output-document=/dev/null "$URL"
    else
        echo "HTTP/4 updater: curl or wget is required" >&2
        return 127
    fi
}

while :; do
    now="$(date -u '+%Y-%m-%dT%H:%M:%SZ')"
    if fetch_once; then
        printf '%s update=ok url=%s\n' "$now" "$URL" >> "$LOG"
    else
        printf '%s update=failed url=%s\n' "$now" "$URL" >> "$LOG"
    fi
    sleep "$INTERVAL_SECONDS"
done
