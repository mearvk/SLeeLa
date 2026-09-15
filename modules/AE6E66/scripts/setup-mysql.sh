#!/usr/bin/env bash
# AE6E66 database preflight.
# Database administration and secret storage remain outside the repository.
set -euo pipefail

DB_NAME="${AE6E66_DB_NAME:-ae6e66}"
DB_USER="${AE6E66_DB_USER:-ae6e66_svc}"

[[ "$DB_NAME" =~ ^[A-Za-z0-9_]+$ ]] || { echo "ERROR: invalid database name." >&2; exit 2; }
[[ "$DB_USER" =~ ^[A-Za-z0-9_]+$ ]] || { echo "ERROR: invalid database user." >&2; exit 2; }

command -v mysql >/dev/null 2>&1 || {
  echo "ERROR: mysql client is not installed." >&2
  exit 1
}

if command -v systemctl >/dev/null 2>&1; then
  if ! systemctl is-active --quiet mysql && ! systemctl is-active --quiet mariadb; then
    echo "ERROR: MySQL/MariaDB is not active." >&2
    exit 1
  fi
fi

cat <<EOF
AE6E66 database preflight: PASS

No database, user, password, schema, or credential file was created or modified.

Administrator requirements:
  database: ${DB_NAME}
  user:     ${DB_USER}
  scope:    localhost only
  grants:   application-specific minimum privileges only
  secrets:  OS secret store / protected runtime file, never Git
  transport: TLS when the database is reached over a network

Use a reviewed migration process to create the schema. Do not execute SQL assembled from untrusted input.
EOF
