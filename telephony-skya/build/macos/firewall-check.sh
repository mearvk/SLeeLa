#!/bin/sh
set -eu
if [ "${SKYA_FIREWALL_CHECK:-1}" = "0" ]; then
  echo "Skya firewall check disabled (SKYA_FIREWALL_CHECK=0)."
  exit 0
fi
echo "Skya firewall preflight (macOS)."
FW="/usr/libexec/ApplicationFirewall/socketfilterfw"
if [ -x "$FW" ]; then
  "$FW" --getglobalstate 2>&1 || true
  "$FW" --getblockall 2>&1 || true
else
  echo "  macOS Application Firewall control utility not available; continuing."
fi
echo "  Note: outbound client connections normally need no inbound rule; private-group/server hosting may require allowing the Skya executable/application."
