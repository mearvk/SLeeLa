#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
"$ROOT/build/macos/firewall-check.sh"
export SKYA_SLEEELA_CIRCUIT="$ROOT/sleela/SkyaClient.sleela"

# The user client flow is GUI -> Guia -> SkyaClient.sleela -> SKYA/1, which uses
# the SLeeLa runtime, not the native C engine. Bring up the SLeeLa side so the
# GUI has a live client to talk to; tear it down on exit.
if "$ROOT/build/macos/sleela-up.sh"; then
    trap '"$ROOT/build/macos/sleela-down.sh" >/dev/null 2>&1 || true' EXIT INT TERM
else
    echo "client.sh: continuing without the SLeeLa side (GUI will show CLIENT.OFFLINE)." >&2
fi

cd "$ROOT/javafx"
mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaClientApp javafx:run
