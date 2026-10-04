#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
"$ROOT/build/linux/firewall-check.sh"
"$ROOT/build/linux/build.sh"
export SKYA_SLEEELA_CIRCUIT="$ROOT/sleela/SkyaClient.sleela"

# Bring up the SLeeLa side (SkyaServer + Guia agent) so the GUI has a live
# client to talk to. Best-effort: if the SLeeLa runtime is not installed the
# GUI still launches and reports CLIENT.OFFLINE for each command.
if "$ROOT/build/linux/sleela-up.sh"; then
    trap '"$ROOT/build/linux/sleela-down.sh" >/dev/null 2>&1 || true' EXIT INT TERM
else
    echo "client.sh: continuing without the SLeeLa side (GUI will show CLIENT.OFFLINE)." >&2
fi

cd "$ROOT/javafx"
mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaClientApp javafx:run
