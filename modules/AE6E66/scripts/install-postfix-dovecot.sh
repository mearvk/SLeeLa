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

for cmd in postconf postconf; do
  command -v "$cmd" >/dev/null 2>&1 || {
    echo "MTA check: '$cmd' is not installed." >&2
    exit 1
  }
done

# Fail closed if the local MTA is configured as an open relay or for broad binding.
relay="$(postconf -h smtpd_relay_restrictions 2>/dev/null || true)"
if [[ "$relay" != *reject_unauth_destination* ]]; then
  echo "ERROR: smtpd_relay_restrictions does not contain reject_unauth_destination." >&2
  exit 1
fi

interfaces="$(postconf -h inet_interfaces 2>/dev/null || true)"
if [[ "$interfaces" == "all" ]]; then
  echo "ERROR: MTA is bound to all interfaces; AE6E66 requires an explicitly reviewed listener." >&2
  exit 1
fi

echo "AE6E66 MTA preflight: PASS"
echo "No package installation, service enablement, firewall change, DNS change, or network binding was performed."
