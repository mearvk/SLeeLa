#!/usr/bin/env bash
# ===========================================================================
# CommonRails cross-engine conformance test.
#
# CommonRails is a shared PRINTING CONTRACT with more than one implementation:
#   - the Sleela library class  lib/common-rails/PrintRail.sleela  (used by
#     Sleela programs, and -- via SLEELA_RAILS -- echoed by the toolchain), and
#   - the native C/C++ "heritage" engine  common-rails/heritage/printing/
#     (linked into the compiler and Nordshrift).
#
# Those two engines must not silently drift. This test renders the SAME cases
# through both and asserts:
#
#   1. SQUARE (the 441-cell progress square -- the core fill contract):
#      native output == Sleela output, BYTE-FOR-BYTE. This is the hard gate.
#
#   2. SINGLE-LINE primitives (component / state lines): each engine's output
#      is pinned to a committed fixture, so ANY change on either side fails
#      the test. The two engines format these lines differently TODAY (the
#      native engine word-wraps at 80 columns and does not inner-pad the
#      [Current:] field or the [STATE] label; the Sleela engine pads both to
#      fixed widths and does not wrap). That difference is INTENTIONALLY
#      recorded in the two fixtures; if the engines are ever converged, update
#      both fixtures in the same change.
#
# Run from the repo root:  bash impl/tests/common_rails/run_common_rails_conformance_tests.sh
# ===========================================================================
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/../../.." && pwd)"
BIN="$ROOT/impl/build/sleela"
HERITAGE="$ROOT/common-rails/heritage/printing/cpp"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

export SLEELA_HOME="$ROOT"
export SLEELA_SHEET="$ROOT/SHEET.sheet"

CXX="${CXX:-g++}"

echo "=== CommonRails cross-engine conformance test ==="

if [[ ! -x "$BIN" ]]; then
  echo "SKIP: $BIN not built (run: make -C impl build/sleela)"
  exit 0
fi

# ---- build + run the native heritage probe --------------------------------
"$CXX" -std=c++17 -I"$HERITAGE" \
  "$HERE/native_probe.cpp" "$HERITAGE/CommonRailsPrinting.cpp" \
  -o "$WORK/native_probe"
"$WORK/native_probe" square > "$WORK/native_square.txt"
"$WORK/native_probe" lines  > "$WORK/native_lines.txt"

# ---- run the Sleela PrintRail probe ---------------------------------------
# Filter the memory-manager / security / defender / verification banner lines
# so only the probe's own output remains.
"$BIN" run "$HERE/sleela_probe.sleela" 2>&1 \
  | grep -vE '^\[(memory-manager|security|defender)\]|^verification|SHA-256|execution/diagnostics' \
  > "$WORK/sleela_full.txt" || true

# The square section is the first 105 lines (5 squares x 21 rows); the line
# section is the remainder. Keep this split in lockstep with the probes.
SQUARE_LINES=105
head -n "$SQUARE_LINES"      "$WORK/sleela_full.txt" > "$WORK/sleela_square.txt"
tail -n +"$((SQUARE_LINES+1))" "$WORK/sleela_full.txt" > "$WORK/sleela_lines.txt"

fail=0

check() { # desc  expected  actual
  # Windows text-mode stdout emits CRLF while committed fixtures are LF.
  # Normalize only line-ending CRs so the byte-content contract is portable.
  local expected_normalized="$WORK/expected.normalized"
  local actual_normalized="$WORK/actual.normalized"
  sed 's/\r$//' "$2" > "$expected_normalized"
  sed 's/\r$//' "$3" > "$actual_normalized"
  if diff -u "$expected_normalized" "$actual_normalized" > "$WORK/diff.txt"; then
    echo "PASS: $1"
  else
    echo "FAIL: $1"
    cat "$WORK/diff.txt"
    fail=1
  fi
}

# 1. The hard gate: the square is byte-identical BETWEEN the two engines...
check "progress square: native engine == Sleela PrintRail (byte-identical)" \
      "$WORK/native_square.txt" "$WORK/sleela_square.txt"
# ...and both still match the committed fixture (catches a lockstep change
# that drifts BOTH engines away from the agreed contract).
check "progress square: native engine == committed fixture" \
      "$HERE/expected_square.txt" "$WORK/native_square.txt"

# 2. Single-line primitives: each engine pinned to its own fixture.
check "native lines == committed native fixture" \
      "$HERE/expected_native_lines.txt" "$WORK/native_lines.txt"
check "Sleela lines == committed Sleela fixture" \
      "$HERE/expected_sleela_lines.txt" "$WORK/sleela_lines.txt"

if [[ "$fail" -ne 0 ]]; then
  echo "CommonRails conformance: FAIL"
  exit 1
fi
echo "CommonRails conformance: PASS"
