#!/bin/bash

# =============================================================================
# regen-manifest.sh — Regenerate the trusted SHA-256 build manifest
# =============================================================================
#
# The verified build gate (impl/Makefile `verify-security`) fails closed unless
# every file listed in security/sha256-manifest.json matches its recorded
# SHA-256. After you INTENTIONALLY edit verified source (the interpreter core,
# front end, Nordshrift driver, catalog, xclass, subject libs, and the extra
# files the build consumes), the manifest must be regenerated to match.
#
# Usage:
#   ./scripts/regen-manifest.sh            # regenerate (after reviewing edits)
#   ./scripts/regen-manifest.sh --check    # drift check only; do NOT rewrite
#
# --check regenerates into a temp file and diffs it against the committed
# manifest, exiting non-zero if they differ. It never modifies anything on
# disk, so it is safe for CI / pre-commit and preserves the fail-closed gate.
#
# IMPORTANT: regenerating blindly defeats the integrity gate. Review your
# source diff first (git status / git diff) and regenerate because the edits
# were intended, not reflexively to silence a verification failure.
# =============================================================================

set -eu

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
GENERATOR="$REPO_ROOT/tools/generate-sha256-manifest.py"
MANIFEST="$REPO_ROOT/security/sha256-manifest.json"
PYTHON="${PYTHON:-python3}"

command -v "$PYTHON" >/dev/null 2>&1 || {
  echo "regen-manifest: FAIL - '$PYTHON' not found" >&2; exit 1; }
[ -r "$GENERATOR" ] || {
  echo "regen-manifest: FAIL - generator not readable: $GENERATOR" >&2; exit 1; }

mode="write"
for arg in "$@"; do
  case "$arg" in
    --check) mode="check" ;;
    -h|--help) sed -n '2,30p' "$0"; exit 0 ;;
    *) echo "regen-manifest: FAIL - unknown argument: $arg" >&2; exit 1 ;;
  esac
done

if [ "$mode" = "check" ]; then
  [ -f "$MANIFEST" ] || {
    echo "regen-manifest: FAIL - committed manifest not found: $MANIFEST" >&2; exit 1; }
  tmp="$(mktemp)"
  trap 'rm -f "$tmp"' EXIT
  "$PYTHON" "$GENERATOR" --root "$REPO_ROOT" --stdout > "$tmp"
  if diff -u "$MANIFEST" "$tmp" >/dev/null 2>&1; then
    echo "regen-manifest: OK - manifest matches the source tree"
    exit 0
  fi
  echo "regen-manifest: FAIL - manifest is out of sync with the source tree" >&2
  echo "  Differing entries (committed vs regenerated):" >&2
  diff -u "$MANIFEST" "$tmp" | grep -E '^[-+][[:space:]]+"' >&2 || true
  echo "  Run './scripts/regen-manifest.sh' (after reviewing your source diff) and commit the result." >&2
  exit 1
fi

"$PYTHON" "$GENERATOR" --root "$REPO_ROOT" --output "$MANIFEST"
echo "regen-manifest: wrote $MANIFEST"
