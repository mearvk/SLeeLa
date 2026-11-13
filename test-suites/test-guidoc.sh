#!/usr/bin/env bash
# Release gate for the document GUI (lib/guidoc).
#   1. gui-document.xml validates against its schema, obeys the cross-field
#      rules, matches SLGuiDocSpec.standard(), claims status=ready, and the claim
#      is verified by running guidoc-demo (no failed check, READY lifecycle, every
#      command run and every menu item clicked).
#   2. the launcher reaches READY, opens a file named by SLEELA_GUIDOC_OPEN, and
#      prints the text outline and the mount plan; an admin can lock the system
#      (SLEELA_GUIDOC_ADMIN=1 SLEELA_GUIDOC_LOCK=1) and a non-admin cannot.
# Needs: python3, xmllint (optional; the validator says so if absent), and the
# sleela binary (default working-binaries/sleela, override with SLEELA=...).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
SLEELA="${SLEELA:-$ROOT/working-binaries/sleela}"
fail() { echo "FAIL: $*" >&2; exit 1; }

python3 tools/validate_gui_document.py --gate --require-ready --sleela "$SLEELA" \
  || fail "gui-document.xml is not a verified ready document"

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT
printf 'first\nsecond\n' > "$WORK/plain.txt"

OUT="$("$SLEELA" run lib/guidoc/guidoc.sleela 2>&1)" || fail "launcher exited non-zero"
echo "$OUT" | grep -q '^GUIDOC READY [0-9][0-9.]* certified' || fail "launcher did not report GUIDOC READY (certified)"
echo "$OUT" | grep -q '^window: SLeeLa Document Editor$' || fail "launcher outline is missing the window line"
echo "$OUT" | grep -q '^launcher notified ready: true$' || fail "the ready event did not fire"
! echo "$OUT" | grep -q '^  FAIL ' || fail "a readiness check failed"

OUT="$(SLEELA_GUIDOC_OPEN="$WORK/plain.txt" SLEELA_GUIDOC_PLAN=1 "$SLEELA" run lib/guidoc/guidoc.sleela 2>&1)" \
  || fail "launcher with a file exited non-zero"
echo "$OUT" | grep -q '\[read-only\]' || fail "a file without a sidecar should open read-only"
echo "$OUT" | grep -q '^mount window win in - role window bg #F7F8FA fg #16202C' || fail "the mount plan is missing the white window"
echo "$OUT" | grep -q '^GUIDOC READY' || fail "launcher with a file did not reach ready"

OUT="$(SLEELA_GUIDOC_ADMIN=1 SLEELA_GUIDOC_LOCK=1 "$SLEELA" run lib/guidoc/guidoc.sleela 2>&1)" \
  || fail "launcher with the lock exited non-zero"
echo "$OUT" | grep -q '^window: SLeeLa Document Editor \[locked\]$' || fail "the locked window title is wrong"
echo "$OUT" | grep -q '^GUIDOC READY [0-9][0-9.]* certified LOCKED ' || fail "the locked launcher did not report LOCKED"
echo "$OUT" | grep -q '^menus: File Edit View System Help$' || fail "the System menu is missing"

OUT="$(SLEELA_GUIDOC_LOCK=1 "$SLEELA" run lib/guidoc/guidoc.sleela 2>&1)" || fail "launcher exited non-zero"
echo "$OUT" | grep -q 'could not lock the system' || fail "a non-admin must not be able to lock"
! echo "$OUT" | grep -q '^GUIDOC READY .* LOCKED ' || fail "a non-admin locked the system"

echo "PASS: document GUI release gate"
