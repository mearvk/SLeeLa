#!/bin/sh
set -eu

PREFIX="${SLEELA_PREFIX:-$HOME/.sleela}"
CONFIG_DIR="$PREFIX/config"
CONFIG_FILE="$CONFIG_DIR/bodi.properties"
BIN_DIR="$PREFIX/bin"
SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
REPO_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)

mkdir -p "$CONFIG_DIR" "$BIN_DIR"

if [ ! -f "$CONFIG_FILE" ]; then
    cat > "$CONFIG_FILE" <<'EOF'
# SLeeLa Bodi network configuration
bind.address=127.0.0.1
rmi.port=8888
bodi.port=8890
bodi.enabled=false
# XML structures that activate the native monitor.
bodi.xml.structures=tree,record,reference,state,command
# Gardulus.II emits local science telemetry when enabled.
bodi.gardulus.enabled=true
bodi.gardulus.interval_ms=1000
EOF
    echo "Created $CONFIG_FILE"
else
    echo "Keeping existing $CONFIG_FILE"
fi

if command -v cc >/dev/null 2>&1; then
    cc -O2 -Wall -Wextra -std=c11 -I"$SCRIPT_DIR" \
        "$SCRIPT_DIR/gardulus_ii.c" "$SCRIPT_DIR/bodisysctl.c" -lm \
        -o "$BIN_DIR/bodisysctl" || echo "Warning: C bodisysctl build unavailable; source remains installed."
fi

if command -v c++ >/dev/null 2>&1; then
    c++ -O2 -Wall -Wextra -std=c++17 -I"$SCRIPT_DIR" \
        "$SCRIPT_DIR/gardulus_ii.c" "$SCRIPT_DIR/bodisysctl.cpp" -lm \
        -o "$BIN_DIR/bodisysctl-cpp" || echo "Warning: C++ bodisysctl build unavailable; source remains installed."
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
C monitor:     $BIN_DIR/bodisysctl
C++ monitor:   $BIN_DIR/bodisysctl-cpp

The Bodi TCP listener remains disabled by default.
Gardulus.II is local telemetry and does not expose a network listener by itself.

Run the C monitor with an integer interval in milliseconds:
  $BIN_DIR/bodisysctl 1000 0

Optional arguments:
  bodisysctl INTERVAL_MS ITERATIONS LOG_FILE UNIX_SOCKET

The monitor unloads naturally when its iteration count is reached, or can be
stopped with SIGINT/SIGTERM. An XML feature may therefore own the monitor's
lifetime without leaving a persistent process behind.
EOF
