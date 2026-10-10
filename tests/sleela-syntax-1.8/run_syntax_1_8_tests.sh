#!/usr/bin/env bash
# =============================================================================
# run_syntax_1_8_tests.sh -- assert the syntax 1.8 surface end to end.
#
# Covers the two 1.8 features and their enforced limits:
#   - inferred local declarations (`let`)
#   - constructor arguments on `new Type(args)`
#
# Usage: run_syntax_1_8_tests.sh <path-to-sleela-binary>
# Run from the repository root (fixtures are referenced relative to this file).
# =============================================================================
set -u

SLEELA="${1:-./impl/build/sleela}"
HERE="$(cd "$(dirname "$0")" && pwd)"
fail=0

# expect_ok <fixture> <label> [<expected-substring-in-stdout>]
expect_ok() {
    out="$("$SLEELA" run "$HERE/$1" 2>/dev/null)"
    rc=$?
    if [ "$rc" -ne 0 ]; then
        echo "  FAIL (should have been accepted) $2"; fail=1; return
    fi
    if [ -n "${3:-}" ] && ! printf '%s' "$out" | grep -qF "$3"; then
        echo "  FAIL (missing expected output '$3') $2"; fail=1; return
    fi
    echo "  ok   (accepted) $2"
}

# expect_reject <fixture> <label> <expected-error-substring>
expect_reject() {
    err="$("$SLEELA" run "$HERE/$1" 2>&1 >/dev/null)"
    if "$SLEELA" run "$HERE/$1" >/dev/null 2>&1; then
        echo "  FAIL (should have been rejected) $2"; fail=1; return
    fi
    if ! printf '%s' "$err" | grep -qF "$3"; then
        echo "  FAIL (wrong rejection message; wanted '$3') $2"; fail=1; return
    fi
    echo "  ok   (rejected) $2"
}

echo "=== syntax 1.8: inferred locals and constructor arguments ==="

echo "accepted:"
expect_ok "let-inference-pass.sleela"       "let inference prints inferred int/string/bool" "hello"
expect_ok "constructor-args-pass.sleela"    "new Point(3,4) runs the declared constructor"  "4"

echo "rejected:"
expect_reject "let-inference-invalid-null.sleela"       "let = null is rejected"                 "cannot infer a usable type"
expect_reject "constructor-arity-invalid.sleela"        "wrong constructor arity is rejected"    "expects a different number of arguments"
expect_reject "constructor-overload-unsupported.sleela" "constructor overloading is rejected"    "duplicate method"

if [ "$fail" -eq 0 ]; then
    echo "SYNTAX 1.8 TESTS: PASS"
    exit 0
else
    echo "SYNTAX 1.8 TESTS: FAIL"
    exit 1
fi
