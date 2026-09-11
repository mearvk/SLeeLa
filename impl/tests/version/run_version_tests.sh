#!/usr/bin/env bash
# =============================================================================
# run_version_tests.sh -- assert the compiler's SL-META-0001 Section 4.4
# version awareness: accepted versions run, out-of-range / malformed versions
# are rejected, and a missing pragma warns but still runs.
#
# Usage: run_version_tests.sh <path-to-sleela-binary>
# =============================================================================
set -u

SLEELA="${1:-./build/sleela}"
HERE="$(cd "$(dirname "$0")" && pwd)"
EX="$HERE/../../examples"
fail=0

expect_ok() {   # expect_ok <file> <label>
    if "$SLEELA" run "$1" >/dev/null 2>&1; then
        echo "  ok   (accepted) $2"
    else
        echo "  FAIL (should have been accepted) $2"; fail=1
    fi
}

expect_reject() {  # expect_reject <file> <label>
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
expect_ok "$EX/versioned.sleela"      "#sleela 1.0 (declared, in range)"
expect_ok "$EX/hello.sleela"          "#sleela 1.0 on classic hello"
expect_ok "$HERE/network_1_1.sleela"  "#sleela 1.1 network built-ins"

echo "rejected:"
expect_reject "$HERE/too_new.sleela"     "#sleela 2.0 (major too new)"
expect_reject "$HERE/minor_ahead.sleela" "#sleela 1.9 (minor too new)"
expect_reject "$HERE/malformed.sleela"   "#sleela one.zero (malformed)"
expect_reject "$HERE/network_too_early.sleela" "#sleela 1.0 using network built-ins"

echo "diagnostics (stderr shown):"
"$SLEELA" run "$HERE/too_new.sleela" 2>&1 >/dev/null | sed 's/^/  > /'
"$SLEELA" run "$HERE/malformed.sleela" 2>&1 >/dev/null | sed 's/^/  > /'

echo "check command (no-run validation):"
"$SLEELA" check "$EX/versioned.sleela" | sed 's/^/  > /'

if [ "$fail" -eq 0 ]; then
    echo "VERSION TESTS: PASS"
    exit 0
else
    echo "VERSION TESTS: FAIL"
    exit 1
fi
