#!/usr/bin/env bash
set -euo pipefail
K="${SLEELA_DB_KIND:-postgresql}"
case "$K" in postgresql) P=libpq;;mysql) P=mysql-client;;mariadb) P=mariadb;;sqlite) P=sqlite;;sqlserver|oracle|odbc) P=unixodbc;;*) echo "unsupported database" >&2;exit 1;;esac
command -v brew >/dev/null||{ echo "Homebrew is required" >&2;exit 1; }
case "${1:-status}" in install) brew install "$P";;upgrade) brew upgrade "$P";;status) brew list --versions "$P" 2>/dev/null||echo "SLeeLa dbctl: client not installed for $K";;*) echo 'Usage: dbctl.sh {install|upgrade|status}';;esac
