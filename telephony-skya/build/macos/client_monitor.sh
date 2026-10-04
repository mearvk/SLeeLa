#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
export SKYA_SLEEELA_CIRCUIT="$ROOT/sleela/SkyaClient.sleela"

# The admin monitor is a Guia client of the running SLeeLa agent; bring the
# SLeeLa side up (server + Guia agent on :8700) and tear it down on exit.
if "$ROOT/build/macos/sleela-up.sh"; then
    trap '"$ROOT/build/macos/sleela-down.sh" >/dev/null 2>&1 || true' EXIT INT TERM
else
    echo "client_monitor.sh: continuing without the SLeeLa side (monitor will show agent offline)." >&2
fi

cd "$ROOT/javafx"
mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaApp javafx:run
