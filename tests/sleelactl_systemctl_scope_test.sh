#!/usr/bin/env sh
# Regression tests for sleelactl's explicit systemd manager selection.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
mkdir -p "$TMP/bin"
# The override registry REPLACES the repository registry, so every module this
# test queries must be listed here (native-drive included). It deliberately uses
# the legacy format-1 layout to keep backward compatibility covered.
cat >"$TMP/registry.conf" <<'EOF'
example daemon example.service Test daemon
native-drive tool tools/sleela-native-drive.py Native Drive test entry
EOF
cat >"$TMP/bin/id" <<'EOF'
#!/bin/sh
printf '%s\n' "${FAKE_UID:-1000}"
EOF
cat >"$TMP/bin/systemctl" <<'EOF'
#!/bin/sh
printf '%s\n' "$*" >>"$SCTL_LOG"
EOF
cat >"$TMP/bin/sudo" <<'EOF'
#!/bin/sh
printf 'called\n' >>"$SUDO_LOG"
exec "$@"
EOF
chmod +x "$TMP/bin/"*
export SCTL_LOG="$TMP/systemctl.log"
export SUDO_LOG="$TMP/sudo.log"
export SLEELACTL_REGISTRY="$TMP/registry.conf"
export PATH="$TMP/bin:$PATH"
: >"$SCTL_LOG"
: >"$SUDO_LOG"

# Helper resolution must return a clean path, not a literal backslash-n suffix.
native_info=$("$ROOT/bin/sleelactl" info native-drive)
printf '%s\n' "$native_info" | grep -Fx '  driver: tools/sleela-native-drive.py' >/dev/null

# User units must use the user manager and must not invoke sudo.
SLEELACTL_SYSTEMCTL_SCOPE=user "$ROOT/bin/sleelactl" start example
grep -Fx -- '--user start example.service' "$SCTL_LOG" >/dev/null
[ ! -s "$SUDO_LOG" ]

# Read-only system-unit status must not require sudo.
SLEELACTL_SYSTEMCTL_SCOPE=system "$ROOT/bin/sleelactl" status example
grep -Fx -- '--system --no-pager status example.service' "$SCTL_LOG" >/dev/null
[ ! -s "$SUDO_LOG" ]

# A registry injected through SLEELACTL_REGISTRY is not trusted for sudo.
before=$(wc -l <"$SCTL_LOG")
if SLEELACTL_SYSTEMCTL_SCOPE=system "$ROOT/bin/sleelactl" enable example 2>"$TMP/untrusted.log"; then
  echo "untrusted override registry was allowed to escalate" >&2; exit 1
fi
grep -F "refusing to escalate with sudo" "$TMP/untrusted.log" >/dev/null
[ ! -s "$SUDO_LOG" ]
[ "$before" -eq "$(wc -l <"$SCTL_LOG")" ]

# Non-root system-unit mutations must explicitly pass through sudo/system scope
# once the operator trusts the override registry.
SLEELACTL_TRUST_REGISTRY=1 SLEELACTL_SYSTEMCTL_SCOPE=system "$ROOT/bin/sleelactl" enable example
grep -Fx -- '--system enable example.service' "$SCTL_LOG" >/dev/null
grep -Fx 'called' "$SUDO_LOG" >/dev/null

# Root system-unit control must not invoke sudo.
before_sudo=$(wc -l <"$SUDO_LOG")
FAKE_UID=0 SLEELACTL_SYSTEMCTL_SCOPE=system "$ROOT/bin/sleelactl" restart example
grep -Fx -- '--system restart example.service' "$SCTL_LOG" >/dev/null
after_sudo=$(wc -l <"$SUDO_LOG")
[ "$before_sudo" -eq "$after_sudo" ]

# Invalid scope must fail closed and must not issue another systemctl command.
before=$(wc -l <"$SCTL_LOG")
if SLEELACTL_SYSTEMCTL_SCOPE=automatic "$ROOT/bin/sleelactl" start example 2>"$TMP/error.log"; then
  echo "invalid systemd scope unexpectedly succeeded" >&2
  exit 1
fi
grep -F "must be 'system' or 'user'" "$TMP/error.log" >/dev/null
after=$(wc -l <"$SCTL_LOG")
[ "$before" -eq "$after" ]

echo "sleelactl systemd scope tests: PASS"
