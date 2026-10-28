#!/usr/bin/env bash
# =============================================================================
# run_version_tests.sh -- assert the compiler's SL-META-0001 Section 4.4
# version awareness against the repository's current syntax contract.
#
# Current supported syntax range: 1.0 .. 2.6.
#
# Usage: run_version_tests.sh <path-to-sleela-binary>
# =============================================================================
set -u

SLEELA="${1:-./build/sleela}"
HERE="$(cd "$(dirname "$0")" && pwd)"
fail=0

expect_ok() {
    if "$SLEELA" run "$1" >/dev/null 2>&1; then
        echo "  ok   (accepted) $2"
    else
        echo "  FAIL (should have been accepted) $2"; fail=1
    fi
}

expect_reject() {
    if "$SLEELA" run "$1" >/dev/null 2>&1; then
        echo "  FAIL (should have been rejected) $2"; fail=1
    else
        echo "  ok   (rejected) $2"
    fi
}

echo "=== version awareness (SL-META-0001 Sec 4.4) ==="
echo "supported range:"
"$SLEELA" version | sed -n '2p'

echo "accepted:"
expect_ok "$HERE/syntax_1_0.sleela"     "#sleela 1.0 (the floor, in range 1.0 .. 2.6)"
expect_ok "$HERE/syntax_1_2.sleela"     "#sleela 1.2 (declared, in range 1.0 .. 2.6)"
expect_ok "$HERE/syntax_1_3.sleela"     "#sleela 1.3 (declared, in range 1.0 .. 2.6)"
expect_ok "$HERE/syntax_1_4.sleela"     "#sleela 1.4 (declared, in range 1.0 .. 2.6)"
expect_ok "$HERE/syntax_missing.sleela" "#sleela absent (defaults to the current version with a warning)"
expect_ok "$HERE/material_2_5.sleela"   "#sleela 2.5 with the 'material' document marker (declarative)"
expect_ok "$HERE/memory_2_5.sleela"     "#sleela 2.5 with the 'memory' document marker (declarative)"
expect_ok "$HERE/wide_integers.sleela"  "#sleela 2.5 with long / int64 / int128 primitives (64-bit)"
expect_ok "$HERE/int_double_widening.sleela" "#sleela 2.5 integer->double widening at decl/assign/field/arg/return"
expect_ok "$HERE/determine_2_6.sleela"  "#sleela 2.6 with the 'determine' document marker (declarative)"

echo "rejected:"
expect_reject "$HERE/syntax_0_9.sleela" "#sleela 0.9 (too old; floor is 1.0)"
expect_reject "$HERE/too_new.sleela"     "#sleela 3.0 (major too new)"
expect_reject "$HERE/minor_ahead.sleela" "#sleela 2.7 (minor too new; ceiling is 2.6)"
expect_reject "$HERE/material_too_early.sleela" "'material' marker under #sleela 2.4 (feature too new for the declared version)"
expect_reject "$HERE/memory_too_early.sleela" "'memory' marker under #sleela 2.4 (feature too new for the declared version)"
expect_reject "$HERE/determine_too_early.sleela" "'determine' marker under #sleela 2.5 (feature too new for the declared version)"
expect_reject "$HERE/malformed.sleela"   "#sleela malformed"

echo "diagnostics (stderr shown):"
"$SLEELA" run "$HERE/syntax_0_9.sleela" 2>&1 >/dev/null | sed 's/^/  > /'
"$SLEELA" run "$HERE/minor_ahead.sleela" 2>&1 >/dev/null | sed 's/^/  > /'
"$SLEELA" run "$HERE/malformed.sleela" 2>&1 >/dev/null | sed 's/^/  > /'

echo "check command (no-run validation):"
"$SLEELA" check "$HERE/syntax_1_3.sleela" | sed 's/^/  > /'

if [ "$fail" -eq 0 ]; then
    echo "VERSION TESTS: PASS"
    exit 0
else
    echo "VERSION TESTS: FAIL"
    exit 1
fi
