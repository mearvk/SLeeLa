#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
"$ROOT/build/linux/build.sh"
export SKYA_SLEEELA_CIRCUIT="$ROOT/sleela/SkyaClient.sleela"
cd "$ROOT/javafx"
exec mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaApp javafx:run
