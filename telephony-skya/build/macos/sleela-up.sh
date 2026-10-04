#!/bin/sh
# sleela-up.sh -- bring up the SLeeLa side of Skya for the GUI (macOS).
#
# Starts the upstream Skya server (SkyaServer.sleela, SKYA/1 on :8443) and the
# Guia control agent (SkyaClient.sleela, Guia/1 on :8700) in the background, so
# the JavaFX GUI has a live SLeeLa client to talk to. The GUI -> Guia -> SLeeLa
# client -> SKYA/1 network -> SLeeLa client -> Guia -> GUI loop needs both.
#
# Writes PIDs to build/macos/.skya-sleela.pids; stop with sleela-down.sh.
# Honors $SLEELA_COMMAND (default: sleela) as the .sleela runner.
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
PIDFILE="$ROOT/build/macos/.skya-sleela.pids"
RUNNER="${SLEELA_COMMAND:-sleela}"

if ! command -v "$RUNNER" >/dev/null 2>&1; then
    echo "sleela-up: SLeeLa runner '$RUNNER' not found on PATH." >&2
    echo "  Build the runtime (make -C impl) and put it on PATH, or set SLEELA_COMMAND." >&2
    exit 127
fi

: > "$PIDFILE"
"$RUNNER" run "$ROOT/sleela/SkyaServer.sleela" >"$ROOT/build/macos/skya-server.log" 2>&1 &
echo $! >> "$PIDFILE"
sleep 1
"$RUNNER" run "$ROOT/sleela/SkyaClient.sleela" >"$ROOT/build/macos/skya-client.log" 2>&1 &
echo $! >> "$PIDFILE"
echo "Skya SLeeLa side up: server (:8443) + Guia agent (:8700). PIDs in $PIDFILE"
