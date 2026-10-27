# SLeeLa antivirus service installation notes
#
# This directory contains a host-side template for the repository's antivirus
# control plane. The package itself remains the source of truth for the
# Sleelavirin policy and scanner logic; the service template is a host
# compatibility layer that should be adapted to the target system.
#
# Copy the example file to your host and adjust values to match the machine:
#
#   sudo install -m 0640 antivirus/systemd/sleela-antivirus.env.example \
#     /etc/sleela-antivirus/sleela-antivirus.env
#
# Then install the unit file:
#
#   sudo install -m 0644 antivirus/systemd/sleela-antivirus.service \
#     /etc/systemd/system/sleela-antivirus.service
#   sudo systemctl daemon-reload
#   sudo systemctl enable --now sleela-antivirus.service
#
# The service is intentionally conservative. It does not silently elevate
# privileges, bypass the fail-closed policy, or assume that every package or
# every host can safely execute a privileged on-access scan by default.

SLEELA_ANTIVIRUS_ENGINE=both
SLEELA_ANTIVIRUS_ROOT=/var/lib/sleela-antivirus
SLEELA_ANTIVIRUS_REPORT=/var/log/sleela-antivirus/scan-report.json
SLEELA_ANTIVIRUS_QUARANTINE=/var/lib/sleela-antivirus/quarantine
SLEELA_ANTIVIRUS_DB=/opt/sleela/antivirus/signatures/native-sha256.txt
SLEELA_ANTIVIRUS_NATIVE=/opt/sleela/antivirus/build/sleelavirin-scan
SLEELA_ANTIVIRUS_SCAN_TIMEOUT=120
SLEELA_ANTIVIRUS_MAX_DEPTH=8
SLEELA_ANTIVIRUS_MAX_FILE_SIZE=100M
