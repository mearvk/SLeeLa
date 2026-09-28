#!/usr/bin/env bash
# AE6E66 local-MTA preflight only.
# This script intentionally does not change Postfix, networking, DNS, or firewall state.
set -euo pipefail

command -v postconf >/dev/null 2>&1 || {
  echo "ERROR: Postfix is not installed." >&2
  exit 1
}

relay="$(postconf -h smtpd_relay_restrictions 2>/dev/null || true)"
interfaces="$(postconf -h inet_interfaces 2>/dev/null || true)"
helo="$(postconf -h smtpd_helo_required 2>/dev/null || true)"

echo "AE6E66 MTA configuration preflight"
echo "  inet_interfaces: ${interfaces:-<unset>}"
echo "  smtpd_helo_required: ${helo:-<unset>}"
echo "  smtpd_relay_restrictions: ${relay:-<unset>}"

[[ "$relay" == *reject_unauth_destination* ]] || {
  echo "ERROR: anti-open-relay protection is missing." >&2
  exit 1
}

[[ "$interfaces" != "all" ]] || {
  echo "ERROR: wildcard network binding requires explicit administrator review." >&2
  exit 1
}

[[ "$helo" == "yes" ]] || {
  echo "ERROR: HELO enforcement is not enabled." >&2
  exit 1
}

echo "AE6E66 MTA preflight: PASS"
echo "No host configuration was changed."
