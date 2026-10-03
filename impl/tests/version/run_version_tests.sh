#!/usr/bin/env bash
# =============================================================================
# run_version_tests.sh -- assert the compiler's SL-META-0001 Section 4.4
# version awareness against the repository's current syntax contract.
#
# Current supported syntax range: 1.3 .. 1.3.
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
expect_ok "$HERE/syntax_1_3.sleela"     "#sleela 1.3 (declared, in range 1.3 .. 1.6)"
expect_ok "$HERE/syntax_1_4.sleela"     "#sleela 1.4 (declared, in range 1.3 .. 1.6)"
expect_ok "$HERE/syntax_missing.sleela" "#sleela absent (defaults to the current version with a warning)"

echo "rejected:"
expect_reject "$HERE/syntax_1_2.sleela" "#sleela 1.2 (too old; floor is 1.3)"
expect_reject "$HERE/too_new.sleela"     "#sleela 2.0 (major too new)"
expect_reject "$HERE/minor_ahead.sleela" "#sleela 1.9 (minor too new; ceiling is 1.6)"
expect_reject "$HERE/malformed.sleela"   "#sleela malformed"

echo "diagnostics (stderr shown):"
"$SLEELA" run "$HERE/syntax_1_2.sleela" 2>&1 >/dev/null | sed 's/^/  > /'
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
