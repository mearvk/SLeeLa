#!/usr/bin/env bash
# SLeeLa / JetBrains source acquisition for macOS
# Max Rupplin - MEARVK LLC - 2026
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec bash "$SCRIPT_DIR/download-source.sh" "$@"
