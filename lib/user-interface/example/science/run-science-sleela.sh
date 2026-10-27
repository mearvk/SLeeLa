#!/bin/bash
# =============================================================================
# run-science-sleela.sh — run every science .sleela program in the SLeeLa runtime
# =============================================================================
#
# These are the SLeeLa-program counterparts of the BODI XML science examples in
# this directory. Each is a runnable .sleela program built on the science
# subject libraries:
#
#   math.sleela          import math;                    (the math facade)
#   physics.sleela       import math; import physics;     (the physics facade)
#   chemistry.sleela     import math; import chemistry;   (the chemistry facade)
#   astrophysics.sleela  import math; import astrophysics; (the astrophysics facade)
#   sociology.sleela     import math; import sociology;   (the sociology facade)
#
# Every program calls its subject library's native facade directly
# (math.sqrt(...), physics.velocity(...), chemistry.ratio(...),
# astrophysics.redshift(...), sociology.rate(...)). Each non-math subject
# depends on math, so every program also imports math.
#
# Usage:
#   ./example/science/run-science-sleela.sh
#   SLEELA_BIN=/path/to/sleela ./example/science/run-science-sleela.sh
# =============================================================================

set -eu

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$REPO_ROOT"

SLEELA="${SLEELA_BIN:-}"
if [ -z "$SLEELA" ]; then
  for cand in "$REPO_ROOT/impl/build/sleela" "$REPO_ROOT/impl/build/sleela.exe" "$REPO_ROOT/bin/sleela"; do
    if [ -x "$cand" ]; then SLEELA="$cand"; break; fi
  done
fi
if [ -z "$SLEELA" ] || [ ! -x "$SLEELA" ]; then
  echo "run-science-sleela: FAIL - sleela binary not found." >&2
  echo "  Build it first:  ./scripts/build-linux.sh   (or: make -C impl)" >&2
  echo "  Or point at one: SLEELA_BIN=/path/to/sleela $0" >&2
  exit 1
fi

export SLEELA_SHEET="${SLEELA_SHEET:-$REPO_ROOT/SHEET.sheet}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$REPO_ROOT/security/sha256-manifest.json}"

echo "run-science-sleela: sleela=$SLEELA"
echo

for prog in math physics chemistry astrophysics sociology; do
  echo "######################################################################"
  echo "## example/science/$prog.sleela"
  echo "######################################################################"
  "$SLEELA" run "example/science/$prog.sleela"
  echo
done

echo "run-science-sleela: all science .sleela programs ran."
