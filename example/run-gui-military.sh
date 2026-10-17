#!/bin/bash

# =============================================================================
# run-gui-military.sh — Run the military-line GUI example in the SLeeLa runtime
# =============================================================================
#
# Convenience wrapper so you don't have to remember the environment the
# fail-closed security gate requires, or list the one-class-per-file military
# dependencies by hand.
#
# It sets, relative to the repository root:
#   SLEELA_SHEET            -> SHEET.sheet          (compiler system sheet)
#   SLEELA_SHA256_MANIFEST  -> the trusted manifest (REQUIRED by the gate;
#                              without it sleela refuses to run)
# and then runs example/gui-military-command-console.sleela together with the
# lib/gui/version-military Master Classes it references.
#
# Usage:
#   ./example/run-gui-military.sh                 # auto-locate the sleela binary
#   SLEELA_BIN=/path/to/sleela ./example/run-gui-military.sh
#
# Any extra arguments are passed through to `sleela run`.
# =============================================================================

set -eu

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

# Locate the built sleela binary (honor SLEELA_BIN if the caller set it).
SLEELA="${SLEELA_BIN:-}"
if [ -z "$SLEELA" ]; then
  for cand in "$REPO_ROOT/impl/build/sleela" "$REPO_ROOT/impl/build/sleela.exe" "$REPO_ROOT/bin/sleela"; do
    if [ -x "$cand" ]; then SLEELA="$cand"; break; fi
  done
fi
if [ -z "$SLEELA" ] || [ ! -x "$SLEELA" ]; then
  echo "run-gui-military: FAIL - sleela binary not found." >&2
  echo "  Build it first:  ./scripts/build-linux.sh   (or: make -C impl)" >&2
  echo "  Or point at one: SLEELA_BIN=/path/to/sleela $0" >&2
  exit 1
fi

# The security gate and the compiler both need these; export so `sleela` sees them.
export SLEELA_SHEET="${SLEELA_SHEET:-$REPO_ROOT/SHEET.sheet}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$REPO_ROOT/security/sha256-manifest.json}"

PROGRAM="example/gui-military-command-console.sleela"
M="lib/gui/version-military"
DEPS="$M/SLPackage.sleela \
$M/SLGuiSettings.sleela \
$M/SLGuiBranding.sleela \
$M/SLGuiKind.sleela \
$M/SLGuiIcon.sleela \
$M/SLGuiCryptoSelector.sleela \
$M/SLGuiCertificateStore.sleela \
$M/SLGuiWindowState.sleela"

echo "run-gui-military: sleela=$SLEELA"
echo "run-gui-military: SLEELA_SHEET=$SLEELA_SHEET"
echo "run-gui-military: SLEELA_SHA256_MANIFEST=$SLEELA_SHA256_MANIFEST"
echo "run-gui-military: running $PROGRAM + 8 military-line classes ..."
echo

# shellcheck disable=SC2086
exec "$SLEELA" run "$PROGRAM" $DEPS "$@"
