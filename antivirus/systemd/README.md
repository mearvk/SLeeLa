# SLeeLa Antivirus Service Configuration
#
# This directory is for host deployment guidance only. The repository gives the
# package contract, while the host-specific values below are the live rules for a
# particular machine.
#
# Recommended deployment commands:
#   sudo install -m 0640 antivirus/systemd/sleela-antivirus.env.example \
#       /etc/sleela-antivirus/sleela-antivirus.env
#   sudo install -m 0644 antivirus/systemd/sleela-antivirus.service \
#       /etc/systemd/system/sleela-antivirus.service
#   sudo systemctl daemon-reload
#   sudo systemctl enable --now sleela-antivirus.service
#
# The host environment file should be owned by root or an admin account, not by a
# working repository checkout. This prevents the packaged source tree from being
# mistaken for the live service policy.
#
# Use the CLI to validate the configuration before enabling the unit:
#   python3 antivirus/tools/sleela-antivirus-ctl.py validate-host --json

SLEELA_ANTIVIRUS_ENGINE=both
SLEELA_ANTIVIRUS_ROOT=/var/lib/sleela-antivirus
SLEELA_ANTIVIRUS_REPORT=/var/log/sleela-antivirus/scan-report.json
SLEELA_ANTIVIRUS_QUARANTINE=/var/lib/sleela-antivirus/quarantine
SLEELA_ANTIVIRUS_DB=/opt/sleela/antivirus/signatures/native-sha256.txt
SLEELA_ANTIVIRUS_NATIVE=/opt/sleela/antivirus/build/sleelavirin-scan
SLEELA_ANTIVIRUS_SCAN_TIMEOUT=120
SLEELA_ANTIVIRUS_MAX_DEPTH=8
SLEELA_ANTIVIRUS_MAX_FILE_SIZE=100M
