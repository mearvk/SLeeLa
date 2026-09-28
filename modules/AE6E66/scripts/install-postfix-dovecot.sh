#!/usr/bin/env bash
# AE6E66 mail integration: validation only.
# This module must not silently install or reconfigure a host MTA.
set -euo pipefail

MODE="${1:---check}"
if [[ "$MODE" != "--check" ]]; then
  echo "AE6E66 does not install Postfix/Dovecot automatically." >&2
  echo "Configure the host MTA independently, then run this script with --check." >&2
  exit 2
fi

command -v postconf >/dev/null 2>&1 || {
  echo "MTA check: 'postconf' is not installed." >&2
  exit 1
}

relay="$(postconf -h smtpd_relay_restrictions 2>/dev/null || true)"
interfaces="$(postconf -h inet_interfaces 2>/dev/null || true)"
helo="$(postconf -h smtpd_helo_required 2>/dev/null || true)"
tls="$(postconf -h smtpd_tls_security_level 2>/dev/null || true)"

[[ "$relay" == *reject_unauth_destination* ]] || {
  echo "ERROR: smtpd_relay_restrictions does not contain reject_unauth_destination." >&2
  exit 1
}
[[ "$interfaces" != "all" ]] || {
  echo "ERROR: MTA is bound to all interfaces; AE6E66 requires an explicitly reviewed listener." >&2
  exit 1
}
[[ "$helo" == "yes" ]] || {
  echo "ERROR: smtpd_helo_required is not enabled." >&2
  exit 1
}
[[ -n "$tls" && "$tls" != "none" ]] || {
  echo "ERROR: SMTP TLS security level is not configured." >&2
  exit 1
}

echo "AE6E66 MTA preflight: PASS"
echo "No package installation, service enablement, firewall change, DNS change, or network binding was performed."
