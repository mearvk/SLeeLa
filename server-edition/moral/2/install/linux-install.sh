#!/usr/bin/env sh
set -eu
umask 077
PREFIX=${SLEEELA_PREFIX:-/opt/sleela}
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../../../.. && pwd)
DEST="$PREFIX/server-edition/moral/2"
install -d -m 0750 "$DEST" "$PREFIX/server-edition/inbox" "$PREFIX/server-edition/state"
cp -R "$ROOT/server-edition/moral/2/." "$DEST/"
cp "$DEST/config/server.conf.example" "$DEST/config/server.conf"
sed -i "s#^engine=.*#engine=$PREFIX/impl/build/sleela#" "$DEST/config/server.conf"
sed -i "s#^inbox=.*#inbox=$PREFIX/server-edition/inbox/requests.txt#" "$DEST/config/server.conf"
sed -i "s#^state=.*#state=$PREFIX/server-edition/state#" "$DEST/config/server.conf"
sed -i "s#^server_source=.*#server_source=$DEST/src/Server.sleela#" "$DEST/config/server.conf"
sed -i "s#^log=.*#log=$PREFIX/server-edition/state/results.log#" "$DEST/config/server.conf"
sed -i "s#^sha256_manifest=.*#sha256_manifest=$PREFIX/security/important-sha256-manifest.json#" "$DEST/config/server.conf"
chmod 0750 "$DEST/serverd"
install -d -m 0755 /etc/systemd/system
sed "s#/opt/sleela#$PREFIX#g" "$DEST/service/sleela-server.service" > /etc/systemd/system/sleela-server.service
command -v systemctl >/dev/null 2>&1 && systemctl daemon-reload || true
printf '%s\n' "Installed SLeeLa Server Edition Service 2 at $DEST"
