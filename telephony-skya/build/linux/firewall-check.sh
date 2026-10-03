#!/bin/sh
set -eu
if [ "${SKYA_FIREWALL_CHECK:-1}" = "0" ]; then
  echo "Skya firewall check disabled (SKYA_FIREWALL_CHECK=0)."
  exit 0
fi
echo "Skya firewall preflight (Linux)."
if [ "$(id -u)" -ne 0 ]; then
  echo "  Non-root preflight: UFW inspection may require elevated privileges; continuing without changing firewall rules."
elif command -v ufw >/dev/null 2>&1; then
  ufw status verbose 2>&1 || true
else
  echo "  UFW is not installed; checking other firewall services is outside this preflight."
fi
echo "  Note: outbound client connections normally need no inbound rule; private-group/server hosting may require an inbound rule for the configured port."
