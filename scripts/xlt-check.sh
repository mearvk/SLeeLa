#!/bin/bash

# =============================================================================
# xlt-check.sh — Verify the XLT compiler and GUI version-line inventories
# =============================================================================
#
# Usage:
#   ./scripts/xlt-check.sh        # run from anywhere; resolves the repo root
#
# What it verifies (fail-closed, exit 1 on any problem):
#   1. The XLT front-end sources and Makefile exist under impl/xlt.
#   2. lib/xlt ships a README and at least one .xlt example.
#   3. If the xltc binary is built, it compiles a congruent model AND rejects a
#      deliberately bad one ("for and against"). If xltc is not built, this is
#      reported as a skipped step rather than a failure.
#   4. Every GUI version line under lib/gui (version-mual/gual/thual/dual/
#      military) carries an SLPackage.sleela manifest and a gui-demo.sleela.
#   5. GUI lines that ship a preview/ directory have a matching .html + README.
#
# Dependencies: bash, standard coreutils. Building xltc (for step 3's live run)
# requires make and a C++17 compiler, but a missing binary only skips step 3.
# =============================================================================

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || { echo "xlt-check: FAIL - cannot cd to repo root" >&2; exit 1; }

fail=0
fail_msg() { echo "xlt-check: FAIL - $1" >&2; fail=1; }

# --- 1. XLT front-end sources --------------------------------------------
XLT_SRC_DIR="impl/xlt"
for f in Makefile xltc.cpp xlt_lexer.cpp xlt_parser.cpp xlt_resolve.cpp \
         xlt_lexer.h xlt_parser.h xlt_resolve.h xlt_model.h xlt_diagnostics.h; do
  if [ ! -f "$XLT_SRC_DIR/$f" ]; then
    fail_msg "missing XLT source: $XLT_SRC_DIR/$f"
  fi
done

# --- 2. XLT library assets and examples ----------------------------------
XLT_LIB_DIR="lib/xlt"
if [ ! -f "$XLT_LIB_DIR/README.md" ]; then
  fail_msg "missing $XLT_LIB_DIR/README.md"
fi
xlt_examples=$(ls "$XLT_LIB_DIR"/examples/*.xlt 2>/dev/null | wc -l | tr -d ' ')
if [ "$xlt_examples" -eq 0 ]; then
  fail_msg "no XLT examples ($XLT_LIB_DIR/examples/*.xlt) found"
fi

# --- 3. xltc behaviour, both directions ----------------------------------
XLTC="$XLT_SRC_DIR/build/xltc"
GOOD="$XLT_LIB_DIR/examples/epyc-ddr2.xlt"
BAD="$XLT_LIB_DIR/examples/bad-model.xlt"
if [ -x "$XLTC" ]; then
  if [ -f "$GOOD" ]; then
    if "$XLTC" "$GOOD" >/dev/null 2>&1; then
      echo "xlt-check: OK - xltc accepts congruent model ($GOOD)"
    else
      fail_msg "xltc rejected a model that should compile: $GOOD"
    fi
  fi
  if [ -f "$BAD" ]; then
    if "$XLTC" "$BAD" >/dev/null 2>&1; then
      fail_msg "xltc accepted a model that should be rejected: $BAD"
    else
      echo "xlt-check: OK - xltc rejects bad model ($BAD)"
    fi
  fi
else
  echo "xlt-check: NOTE - $XLTC not built; skipping live compile check" >&2
  echo "          build it with: make -C $XLT_SRC_DIR" >&2
fi

# --- 4. GUI version-line manifests ---------------------------------------
gui_lines=0
for g in lib/gui/version-mual lib/gui/version-gual lib/gui/version-thual \
         lib/gui/version-dual lib/gui/version-military; do
  [ -d "$g" ] || { fail_msg "GUI version line missing: $g"; continue; }
  gui_lines=$((gui_lines + 1))
  [ -f "$g/SLPackage.sleela" ] || fail_msg "missing $g/SLPackage.sleela"
  [ -f "$g/gui-demo.sleela" ]  || fail_msg "missing $g/gui-demo.sleela"
done

# --- 5. GUI preview assets (only for lines that ship one) ----------------
for p in lib/gui/version-*/preview; do
  [ -d "$p" ] || continue
  html=$(ls "$p"/*.html 2>/dev/null | wc -l | tr -d ' ')
  if [ "$html" -eq 0 ]; then
    fail_msg "preview dir has no .html render: $p"
  fi
  if [ ! -f "$p/README.md" ]; then
    fail_msg "preview dir missing README.md: $p"
  fi
done

# --- Result ---------------------------------------------------------------
if [ "$fail" -ne 0 ]; then
  echo "xlt-check: FAILED" >&2
  exit 1
fi

echo "xlt-check: OK - XLT front-end present, $xlt_examples .xlt example(s), $gui_lines GUI version line(s)"
exit 0
