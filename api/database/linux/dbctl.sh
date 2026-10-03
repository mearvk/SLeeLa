#!/usr/bin/env bash
set -euo pipefail
K="${SLEELA_DB_KIND:-postgresql}"
case "$K" in postgresql) P=postgresql-client;;mysql) P=mysql-client;;mariadb) P=mariadb-client;;sqlite) P=sqlite3;;sqlserver|oracle|odbc) P=unixodbc;;*) echo "unsupported database" >&2;exit 1;;esac
install_db(){ if command -v apt-get >/dev/null;then sudo apt-get update;sudo apt-get install -y "$P";elif command -v dnf >/dev/null;then sudo dnf install -y "$P";elif command -v yum >/dev/null;then sudo yum install -y "$P";else echo "no supported package manager" >&2;exit 1;fi; }
upgrade_db(){ if command -v apt-get >/dev/null;then sudo apt-get update;sudo apt-get install --only-upgrade -y "$P";elif command -v dnf >/dev/null;then sudo dnf upgrade -y "$P";elif command -v yum >/dev/null;then sudo yum update -y "$P";else echo "no supported package manager" >&2;exit 1;fi; }
case "${1:-status}" in install) install_db;;upgrade) upgrade_db;;status) command -v psql mysql mariadb sqlite3 sqlcmd sqlplus odbcinst 2>/dev/null||true;echo "SLeeLa dbctl: inspect the configured native client/driver for $K";;*) echo 'Usage: dbctl.sh {install|upgrade|status}';;esac
