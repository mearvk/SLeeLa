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
bind.address=127.0.0.1
rmi.port=8888
bodi.port=8890
bodi.enabled=false
bodi.xml.structures=tree,record,reference,state,command
bodi.gardulus.enabled=true
bodi.gardulus.interval_ms=1000
EOF
fi
if command -v cc >/dev/null 2>&1; then cc -O2 -Wall -Wextra -std=c11 -I"$SCRIPT_DIR" "$SCRIPT_DIR/gardulus_ii.c" "$SCRIPT_DIR/bodisysctl.c" -lm -o "$BIN_DIR/bodisysctl" || echo "Warning: C build unavailable; source remains installed."; fi
if command -v c++ >/dev/null 2>&1; then c++ -O2 -Wall -Wextra -std=c++17 -I"$SCRIPT_DIR" "$SCRIPT_DIR/gardulus_ii.c" "$SCRIPT_DIR/bodisysctl.cpp" -lm -o "$BIN_DIR/bodisysctl-cpp" || echo "Warning: C++ build unavailable; source remains installed."; fi
cat > "$PREFIX/bodi-network.sh" <<EOF
#!/bin/sh
set -eu
exec java -cp "$REPO_ROOT/src" implementations._001_.bodi.BodiNetworkBootstrap "$CONFIG_FILE"
EOF
chmod 700 "$PREFIX/bodi-network.sh"
printf '%s\n' "Bodi installation prepared." "Configuration: $CONFIG_FILE" "C monitor: $BIN_DIR/bodisysctl" "C++ monitor: $BIN_DIR/bodisysctl-cpp" "Run: $BIN_DIR/bodisysctl 1000 0" "The Bodi TCP listener remains disabled by default."