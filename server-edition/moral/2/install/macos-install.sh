#!/usr/bin/env sh
set -eu
PREFIX=${SLEEELA_PREFIX:-/opt/sleela}
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../../../.. && pwd)
DEST="$PREFIX/server-edition/moral/2"
install -d -m 0750 "$DEST" "$PREFIX/server-edition/inbox" "$PREFIX/server-edition/state"
cp -R "$ROOT/server-edition/moral/2/." "$DEST/"
install -d -m 0750 "$PREFIX/server-edition/port-awareness"
cp -R "$ROOT/server-edition/port-awareness/." "$PREFIX/server-edition/port-awareness/"
chmod 0750 "$PREFIX/server-edition/port-awareness/portctl.sh"
cp "$DEST/config/server.conf.example" "$DEST/config/server.conf"
sed -i '' "s#^engine=.*#engine=$PREFIX/impl/build/sleela#" "$DEST/config/server.conf"
sed -i '' "s#^inbox=.*#inbox=$PREFIX/server-edition/inbox/requests.txt#" "$DEST/config/server.conf"
sed -i '' "s#^state=.*#state=$PREFIX/server-edition/state#" "$DEST/config/server.conf"
sed -i '' "s#^server_source=.*#server_source=$DEST/src/Server.sleela#" "$DEST/config/server.conf"
sed -i '' "s#^log=.*#log=$PREFIX/server-edition/state/results.log#" "$DEST/config/server.conf"
PLIST="$HOME/Library/LaunchAgents/com.mearvk.sleela-server.plist"
install -d -m 0700 "$HOME/Library/LaunchAgents"
sed "s#/opt/sleela#$PREFIX#g" "$DEST/service/com.mearvk.sleela-server.plist" > "$PLIST"
chmod 0700 "$DEST/serverd"
printf '%s\n' "Installed launchd definition at $PLIST"
