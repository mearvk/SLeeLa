#!/usr/bin/env sh
# mt-check — CI gate for the MT metalayers.
#
# Runs two passwordless checks and fails (non-zero) if either fails:
#   1. metatag scan  — every @MT: marker is legal and in canonical order
#                      (MT-META-0001).
#   2. seal audit    — every sealed document still matches its recorded solve
#                      matrix (MT-SEAL-0001). Catches tampering WITHOUT any
#                      password: the stored matrix pins each document's shape.
#
# Usage (from anywhere): impl/ci/mt-check.sh
# Exit 0 = all green; non-zero = a violation or a broken seal.

set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
REPO_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)

rc=0

echo "== MT-META-0001: metatag scan =="
if sh "$REPO_ROOT/impl/metatag/metatag.sh" scan; then
  echo "   metatags OK"
else
  echo "   metatag scan FAILED" >&2
  rc=1
fi

echo
echo "== MT-SEAL-0001: seal audit (passwordless integrity) =="
if python3 "$REPO_ROOT/impl/seal/seal.py" audit; then
  echo "   seals OK"
else
  echo "   seal audit FAILED (a sealed document was altered)" >&2
  rc=1
fi

echo
if [ "$rc" -eq 0 ]; then
  echo "mt-check: ALL GREEN"
else
  echo "mt-check: FAILURE" >&2
fi
exit "$rc"
