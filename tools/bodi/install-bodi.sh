#!/bin/sh
set -eu

PREFIX="${SLEELA_PREFIX:-$HOME/.sleela}"
CONFIG_DIR="$PREFIX/config"
CONFIG_FILE="$CONFIG_DIR/bodi.properties"
SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
REPO_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)

mkdir -p "$CONFIG_DIR"

if [ ! -f "$CONFIG_FILE" ]; then
    cat > "$CONFIG_FILE" <<'EOF'
# SLeeLa Bodi network configuration
bind.address=127.0.0.1
rmi.port=8888
bodi.port=8890
bodi.enabled=false
EOF
    echo "Created $CONFIG_FILE"
else
    echo "Keeping existing $CONFIG_FILE"
fi

cat > "$PREFIX/bodi-network.sh" <<EOF
#!/bin/sh
set -eu
exec java -cp "$REPO_ROOT/src" implementations._001_.bodi.BodiNetworkBootstrap "$CONFIG_FILE"
EOF
chmod 700 "$PREFIX/bodi-network.sh"

cat <<EOF
Bodi installation prepared.

Configuration: $CONFIG_FILE
Launcher:      $PREFIX/bodi-network.sh

The TCP listener is disabled by default.
To enable local communication, set:
  bodi.enabled=true

For a deliberately exposed interface, change bind.address explicitly and
apply the host firewall policy appropriate to that interface.
EOF
