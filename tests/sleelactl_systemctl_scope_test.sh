#!/usr/bin/env sh
# Regression tests for sleelactl's explicit systemd manager selection.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
mkdir -p "$TMP/bin"
cat >"$TMP/registry.conf" <<'EOF'
example daemon example.service Test daemon
EOF
cat >"$TMP/bin/id" <<'EOF'
#!/bin/sh
printf '1000\n'
EOF
cat >"$TMP/bin/systemctl" <<'EOF'
#!/bin/sh
printf '%s\n' "$*" >>"$SCTL_LOG"
EOF
cat >"$TMP/bin/sudo" <<'EOF'
#!/bin/sh
printf 'called\\n' >>"$SUDO_LOG"
exec "$@"
EOF
chmod +x "$TMP/bin/"*
export SCTL_LOG="$TMP/systemctl.log"
export SUDO_LOG="$TMP/sudo.log"
export SLEELACTL_REGISTRY="$TMP/registry.conf"
export PATH="$TMP/bin:$PATH"
: >"$SCTL_LOG"
: >"$SUDO_LOG"

# User units must use the user manager and must not invoke sudo.
SLEELACTL_SYSTEMCTL_SCOPE=user "$ROOT/bin/sleelactl" start example
grep -Fx -- '--user start example.service' "$SCTL_LOG" >/dev/null
[ ! -s "$SUDO_LOG" ]

# Non-root system-unit control must explicitly pass through sudo/system scope.
SLEELACTL_SYSTEMCTL_SCOPE=system "$ROOT/bin/sleelactl" enable example
grep -Fx -- '--system enable example.service' "$SCTL_LOG" >/dev/null
grep -Fx 'called' "$SUDO_LOG" >/dev/null

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
