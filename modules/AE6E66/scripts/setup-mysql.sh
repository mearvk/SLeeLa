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

# Inspect grants without printing them, so a privileged check cannot leak
# password hashes or other authentication material into normal output.
grant_file="$(mktemp)"
trap 'rm -f "$grant_file"' EXIT
mysql --batch --skip-column-names -e "SHOW GRANTS FOR '${DB_USER}'@'localhost'" >"$grant_file" 2>/dev/null || {
  echo "ERROR: unable to inspect service-account grants." >&2
  exit 1
}
if grep -Eiq '(^|[[:space:]])GRANT ALL PRIVILEGES|GRANT FILE|GRANT SUPER|GRANT SYSTEM_|GRANT PROCESS|GRANT SHUTDOWN|GRANT RELOAD' "$grant_file"; then
  echo "ERROR: service account has prohibited administrative privileges." >&2
  exit 1
fi

# A local deployment may still use encrypted transport. Record only whether
# the server advertises TLS support; do not print credentials or session data.
have_ssl="$(mysql --batch --skip-column-names -e 'SELECT @@have_ssl' 2>/dev/null || true)"
if [[ "$have_ssl" != "YES" && "${AE6E66_DB_TLS_REQUIRED:-true}" == "true" ]]; then
  echo "ERROR: database TLS support is unavailable while TLS is required." >&2
  exit 1
fi

echo "AE6E66 database preflight: PASS"
echo "  database: ${DB_NAME}"
echo "  user:     ${DB_USER}"
echo "  scope:    local host only"
echo "  secrets:  external OS secret store / protected runtime only"
echo "  schema:   reviewed migration process required"
echo "No database, user, password, schema, or credential file was created or modified."
