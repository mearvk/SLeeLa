#!/bin/bash
# =============================================================================
# run-gui-examples.sh — run every GUI-line example in the SLeeLa runtime
# =============================================================================
#
# This workspace ships one GUI example per lib/gui version line. Each one now
# renders a REAL, on-screen window through SleelaUI (SLeeLa's own native C/C++
# toolkit) via the ui* SLVM bridge -- it no longer merely prints a walkthrough:
#
#   gui-mual-text-editor.sleela         MUAL      1.0  (baseline)
#   gui-gual-settings-console.sleela    GUAL      2.0  (hardened settings-first)
#   gui-thual-effects-showcase.sleela   THUAL     3.0  (visual effects)
#   gui-dual-media-studio.sleela        DUAL      4.0  (rich components)
#   gui-military-command-console.sleela MILITARY  5.0  (military hardening)
#
# Each is self-contained (inline model + main, only ui* built-ins), so it runs
# on the toolchain as it is today without passing any dependency classes. On a
# host with a display (X on Linux, native on macOS/Windows) each opens a window
# and paints its widgets; headless/CI it reports "no display" and exits 0.
# This wrapper sets the environment the fail-closed SHA-256 security gate
# requires and runs them all.
#
# Usage:
#   ./example/run-gui-examples.sh
#   SLEELA_BIN=/path/to/sleela ./example/run-gui-examples.sh
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
  echo "run-gui-examples: FAIL - sleela binary not found." >&2
  echo "  Build it first:  ./scripts/build-linux.sh   (or: make -C impl)" >&2
  echo "  Or point at one: SLEELA_BIN=/path/to/sleela $0" >&2
  exit 1
fi

export SLEELA_SHEET="${SLEELA_SHEET:-$REPO_ROOT/SHEET.sheet}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$REPO_ROOT/security/sha256-manifest.json}"

echo "run-gui-examples: sleela=$SLEELA"
echo "run-gui-examples: SLEELA_SHEET=$SLEELA_SHEET"
echo "run-gui-examples: SLEELA_SHA256_MANIFEST=$SLEELA_SHA256_MANIFEST"
echo

EXAMPLES=(
  "example/mual/gui-mual-text-editor.sleela"
  "example/gual/gui-gual-settings-console.sleela"
  "example/thual/gui-thual-effects-showcase.sleela"
  "example/dual/gui-dual-media-studio.sleela"
  "example/military/gui-military-command-console.sleela"
)

for prog in "${EXAMPLES[@]}"; do
  echo "######################################################################"
  echo "## $prog"
  echo "######################################################################"
  "$SLEELA" run "$prog"
  echo
done

echo "run-gui-examples: all GUI-line examples ran."
echo "On a host with a display each opens a real SleelaUI window; headless they"
echo "report 'no display' and exit 0. The DESIGNED military window variant"
echo "Running the additional native-window demos..."
"$SLEELA" run "military/gui-military-window.sleela"
"$SLEELA" run "demo/gui-buffering-resize.sleela"
echo "run-gui-examples: all GUI and native-window examples ran."
