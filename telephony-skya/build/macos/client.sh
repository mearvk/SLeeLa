#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
"$ROOT/build/macos/build.sh"
export SKYA_SLEEELA_CIRCUIT="$ROOT/sleela/SkyaClient.sleela"
cd "$ROOT/javafx"
exec mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaClientApp javafx:run
