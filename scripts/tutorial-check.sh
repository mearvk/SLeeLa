#!/bin/bash

# =============================================================================
# tutorial-check.sh — Verify the tutorial and examples inventories
# =============================================================================
#
# Usage:
#   ./scripts/tutorial-check.sh        # run from the repository root
#   make tutorial-check                # via the root build dispatcher
#
# What it verifies (fail-closed, exit 1 on any problem):
#   1. tutorial/README.md exists and every lesson it links resolves to a file.
#   2. The tutorial lessons are a contiguous, zero-padded 01..NN sequence.
#   3. examples/ contains at least one .xml example.
#   4. Every example .xml is well-formed XML (xmllint, else Python fallback).
#   5. Every example family that examples/README.md says has expected evidence
#      has its matching .expected.* witness file on disk, and every .expected.*
#      file on disk has a corresponding .xml example (no orphans).
#
# Dependencies: bash, grep, sed, sort. XML well-formedness uses xmllint when
# present and falls back to python3; if neither is available that single check
# is skipped with a notice rather than failing the build.
# =============================================================================

set -u

# Resolve the repository root from this script's location so the check works
# regardless of the caller's current directory.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || { echo "tutorial-check: FAIL - cannot cd to repo root" >&2; exit 1; }

TUT_DIR="tutorial"
EX_DIR="examples"
fail=0

fail_msg() { echo "tutorial-check: FAIL - $1" >&2; fail=1; }

# --- 1. tutorial/README.md and its linked lessons -------------------------
if [ ! -f "$TUT_DIR/README.md" ]; then
  fail_msg "missing $TUT_DIR/README.md inventory"
  linked=""
else
  # Markdown links of the form (NN-slug.md); de-duplicate, keep order.
  linked=$(grep -oE '\(([0-9]+-[a-z0-9-]+\.md)\)' "$TUT_DIR/README.md" \
             | tr -d '()' | awk '!seen[$0]++')
  if [ -z "$linked" ]; then
    fail_msg "no lesson links found in $TUT_DIR/README.md"
  fi
fi

tut_linked=0
for lesson in $linked; do
  tut_linked=$((tut_linked + 1))
  if [ ! -f "$TUT_DIR/$lesson" ]; then
    fail_msg "lesson linked in README is missing on disk: $TUT_DIR/$lesson"
  fi
done

# --- 2. contiguous, zero-padded lesson numbering --------------------------
nums=$(ls "$TUT_DIR"/[0-9][0-9]-*.md 2>/dev/null | sed -E 's#.*/([0-9]+)-.*#\1#' | sort -n | uniq)
tut_files=0
expected=1
for n in $nums; do
  tut_files=$((tut_files + 1))
  want=$(printf '%02d' "$expected")
  if [ "$n" != "$want" ]; then
    fail_msg "lesson numbering gap: expected $want, found $n"
  fi
  expected=$((expected + 1))
done
if [ "$tut_files" -eq 0 ]; then
  fail_msg "no numbered tutorial lessons ($TUT_DIR/NN-*.md) found"
fi

# --- 3. examples present --------------------------------------------------
ex_xml=$(ls "$EX_DIR"/*.xml 2>/dev/null | wc -l | tr -d ' ')
if [ "$ex_xml" -eq 0 ]; then
  fail_msg "no example projects ($EX_DIR/*.xml) found"
fi

# --- 4. XML well-formedness ----------------------------------------------
xml_checker=""
if command -v xmllint >/dev/null 2>&1; then
  xml_checker="xmllint"
elif command -v python3 >/dev/null 2>&1; then
  xml_checker="python3"
fi

if [ -z "$xml_checker" ]; then
  echo "tutorial-check: NOTE - neither xmllint nor python3 found; skipping XML well-formedness check" >&2
else
  for xml in "$EX_DIR"/*.xml; do
    [ -e "$xml" ] || continue
    if [ "$xml_checker" = "xmllint" ]; then
      if ! xmllint --noout "$xml" 2>/dev/null; then
        fail_msg "malformed XML: $xml"
      fi
    else
      if ! python3 -c 'import sys,xml.dom.minidom as m; m.parse(sys.argv[1])' "$xml" 2>/dev/null; then
        fail_msg "malformed XML: $xml"
      fi
    fi
  done
fi

# --- 5. expected-evidence pairing ----------------------------------------
# README families that declare an expected-evidence file: the README names the
# witness filename directly (e.g. `07-post.expected.txt`). Each named witness
# must exist, and each witness on disk must pair with a sibling .xml example.
if [ -f "$EX_DIR/README.md" ]; then
  declared=$(grep -oE '[0-9]+-[a-z0-9-]+\.expected\.[a-z0-9]+' "$EX_DIR/README.md" \
               | awk '!seen[$0]++')
  for w in $declared; do
    if [ ! -f "$EX_DIR/$w" ]; then
      fail_msg "README declares expected evidence that is missing: $EX_DIR/$w"
    fi
  done
fi

# Every .expected.* on disk must have a matching .xml example (no orphans).
exp_count=0
for exp in "$EX_DIR"/*.expected.*; do
  [ -e "$exp" ] || continue
  exp_count=$((exp_count + 1))
  base=$(basename "$exp")
  stem="${base%%.expected.*}"
  if [ ! -f "$EX_DIR/$stem.xml" ]; then
    fail_msg "expected-evidence file has no matching example: $exp (want $EX_DIR/$stem.xml)"
  fi
done

# --- Result ---------------------------------------------------------------
if [ "$fail" -ne 0 ]; then
  echo "tutorial-check: FAILED" >&2
  exit 1
fi

echo "tutorial-check: OK - $tut_files tutorial lesson(s) ($tut_linked linked in README), $ex_xml example(s), $exp_count expected-evidence witness(es)"
exit 0
