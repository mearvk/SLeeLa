#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
"$ROOT/build/linux/firewall-check.sh"
export SKYA_SLEEELA_CIRCUIT="$ROOT/sleela/SkyaClient.sleela"

# Read the Guia control endpoint from config/skya.conf so the GUI and agent
# agree on where Guia is served. The Java GUI honors SKYA_GUIA_HOST/PORT.
CONF="$ROOT/config/skya.conf"
if [ -f "$CONF" ]; then
    GUIA_HOST=$(sed -n 's/^control\.guia\.host=//p' "$CONF" | head -n1)
    GUIA_PORT=$(sed -n 's/^control\.guia\.port=//p' "$CONF" | head -n1)
    SRV_HOST=$(sed -n 's/^listen\.host=//p' "$CONF" | head -n1)
    SRV_PORT=$(sed -n 's/^listen\.port=//p' "$CONF" | head -n1)
    DEF_ROOM=$(sed -n 's/^default\.room=//p' "$CONF" | head -n1)
    [ -n "${GUIA_HOST:-}" ] && export SKYA_GUIA_HOST="$GUIA_HOST"
    [ -n "${GUIA_PORT:-}" ] && export SKYA_GUIA_PORT="$GUIA_PORT"
    [ -n "${SRV_HOST:-}" ] && export SKYA_SERVER_HOST="$SRV_HOST"
    [ -n "${SRV_PORT:-}" ] && export SKYA_SERVER_PORT="$SRV_PORT"
    [ -n "${DEF_ROOM:-}" ] && export SKYA_DEFAULT_ROOM="$DEF_ROOM"
    CRYPTO=$(sed -n 's/^security\.crypto\.enabled=//p' "$CONF" | head -n1)
    [ "${CRYPTO:-}" = "false" ] && export SKYA_CRYPTO=off
fi

# The user client flow is GUI -> Guia -> SkyaClient.sleela -> SKYA/1, which uses
# the SLeeLa runtime, not the native C engine. (Build the native engine
# separately with build.sh / make -C native when you want the standalone
# skya/skya-server binaries.) Bring up the SLeeLa side so the GUI has a live
# client to talk to. Best-effort: if the SLeeLa runtime is not installed the
# GUI still launches and reports CLIENT.OFFLINE for each command.
if "$ROOT/build/linux/sleela-up.sh"; then
    trap '"$ROOT/build/linux/sleela-down.sh" >/dev/null 2>&1 || true' EXIT INT TERM
else
    echo "client.sh: continuing without the SLeeLa side (GUI will show CLIENT.OFFLINE)." >&2
fi

cd "$ROOT/javafx"
mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaClientApp javafx:run
