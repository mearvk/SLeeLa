#!/usr/bin/env sh
set -eu
PREFIX=${SLEEELA_PREFIX:-/opt/sleela}
PLIST="$HOME/Library/LaunchAgents/com.mearvk.sleela-server.plist"
launchctl bootout "gui/$UID" "$PLIST" 2>/dev/null || true
rm -f "$PLIST"
rm -rf "$PREFIX/server-edition/moral/2"
printf '%s\n' "Removed SLeeLa Server Edition Service 2"
