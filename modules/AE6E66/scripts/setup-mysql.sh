#!/usr/bin/env bash
# AE6E66 database security preflight only.
# No users, passwords, schema, or credentials are created or modified.
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

# These read-only checks intentionally require an administrator-authorized
# local client configuration or socket authentication. No password is accepted
# on the command line and no secret is printed.
status="$(mysql --batch --skip-column-names -e 'SELECT @@version, @@have_ssl' 2>/dev/null || true)"
if [[ -z "$status" ]]; then
  echo "ERROR: unable to establish a local administrative database check." >&2
  exit 1
fi

if ! mysql --batch --skip-column-names -e "SHOW DATABASES" 2>/dev/null | grep -Fxq "$DB_NAME"; then
  echo "ERROR: database does not exist: $DB_NAME" >&2
  exit 1
fi

host_grants="$(mysql --batch --skip-column-names -e "SELECT User,Host FROM mysql.user WHERE User='${DB_USER}'" 2>/dev/null || true)"
if [[ -z "$host_grants" ]]; then
  echo "ERROR: service account does not exist: $DB_USER" >&2
  exit 1
fi
if ! awk -F '\t' '$2 == "localhost" || $2 == "127.0.0.1" || $2 == "::1" {ok=1} END {exit ok ? 0 : 1}' <<< "$host_grants"; then
  echo "ERROR: service account is not restricted to a local host." >&2
  exit 1
fi

echo "AE6E66 database preflight: PASS"
echo "  database: ${DB_NAME}"
echo "  user:     ${DB_USER}"
echo "  secrets:  external OS secret store / protected runtime only"
echo "  schema:   reviewed migration process required"
echo "No database, user, password, schema, or credential file was created or modified."
