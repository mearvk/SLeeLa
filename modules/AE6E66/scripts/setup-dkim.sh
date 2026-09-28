#!/usr/bin/env bash
# AE6E66 DKIM integration preflight.
# Host DNS, MTA, and key management are administrator responsibilities.
set -euo pipefail

CONFIG="${AE6E66_MAIL_CONFIG:-}"
if [[ -z "$CONFIG" || ! -f "$CONFIG" || -L "$CONFIG" ]]; then
  echo "ERROR: set AE6E66_MAIL_CONFIG to an administrator-controlled regular mail configuration file." >&2
  echo "This script intentionally contains no domain, IP, selector, password, or private key." >&2
  exit 2
fi

command -v postconf >/dev/null 2>&1 || {
  echo "ERROR: Postfix postconf not found." >&2
  exit 1
}

interfaces="$(postconf -h inet_interfaces 2>/dev/null || true)"
relay="$(postconf -h smtpd_relay_restrictions 2>/dev/null || true)"
milters="$(postconf -h smtpd_milters 2>/dev/null || true)"
tls="$(postconf -h smtpd_tls_security_level 2>/dev/null || true)"

[[ "$interfaces" != "all" ]] || { echo "ERROR: wildcard MTA listener." >&2; exit 1; }
[[ "$relay" == *reject_unauth_destination* ]] || { echo "ERROR: open-relay protection missing." >&2; exit 1; }
[[ "$milters" == *unix:* ]] || { echo "ERROR: expected a local Unix-socket DKIM milter." >&2; exit 1; }
[[ -n "$tls" && "$tls" != "none" ]] || { echo "ERROR: SMTP TLS security level is not configured." >&2; exit 1; }

echo "AE6E66 DKIM/MTA preflight: PASS"
echo "No package installation, key generation, DNS modification, service restart, or Postfix reconfiguration was performed."
