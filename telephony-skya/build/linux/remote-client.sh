#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT/javafx"
exec mvn -q -DmainClass=com.mearvk.sleela.skya.SkyaConnectApp javafx:run
