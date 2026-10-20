#!/bin/bash

# =============================================================================
# run-gui-military.sh — Run the military-line GUI example in the SLeeLa runtime
# =============================================================================
#
# Convenience wrapper so you don't have to remember the environment the
# fail-closed security gate requires.
#
# It sets, relative to the repository root:
#   SLEELA_SHEET            -> SHEET.sheet          (compiler system sheet)
#   SLEELA_SHA256_MANIFEST  -> the trusted manifest (REQUIRED by the gate;
#                              without it sleela refuses to run)
# and then runs example/military/gui-military-command-console.sleela, which now renders a
# REAL, on-screen window through SleelaUI (the native ui* toolkit) rather than
# printing a walkthrough. It is self-contained (only ui* built-ins), so no
# dependency classes are passed on the command line.
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

PROGRAM="example/military/gui-military-command-console.sleela"

echo "run-gui-military: sleela=$SLEELA"
echo "run-gui-military: SLEELA_SHEET=$SLEELA_SHEET"
echo "run-gui-military: SLEELA_SHA256_MANIFEST=$SLEELA_SHA256_MANIFEST"
echo "run-gui-military: running $PROGRAM ..."
echo

# The program is self-contained (only ui* built-ins driving SleelaUI), so no
# dependency classes are listed here. On a host with a display it opens a real
# window; headless it reports "no display" and exits 0.
# shellcheck disable=SC2086
exec "$SLEELA" run "$PROGRAM" "$@"
