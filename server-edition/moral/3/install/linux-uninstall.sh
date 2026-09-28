#!/usr/bin/env sh
set -eu
PREFIX=${SLEEELA_PREFIX:-/opt/sleela}
command -v systemctl >/dev/null 2>&1 && systemctl disable --now sleela-server3.service 2>/dev/null || true
rm -f /etc/systemd/system/sleela-server3.service
rm -rf "$PREFIX/server-edition/moral/3"
printf '%s\n' "Removed SLeeLa Server Edition Service 3"
