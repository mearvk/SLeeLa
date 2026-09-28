#!/usr/bin/env bash
# =============================================================================
# run_subject_tests.sh -- numeric assertions for the subject libraries.
#
# Each *_values.sleela program computes known quantities and prints one
# "PASS <name>" or "FAIL <name> ..." line per check. This harness runs every
# such program on the built `sleela` binary and fails if:
#   * the program does not run cleanly,
#   * any "FAIL" line is emitted, or
#   * the number of "PASS" lines differs from the EXPECTED_PASS=<N> marker
#     recorded in the program (guards against a check silently not running).
#
# Usage: run_subject_tests.sh <path-to-sleela-binary>
# =============================================================================
set -u

SLEELA="${1:-./build/sleela}"
# Resolve to an absolute path so it works after we cd to the repo root.
case "$SLEELA" in /*) : ;; *) SLEELA="$(cd "$(dirname "$SLEELA")" && pwd)/$(basename "$SLEELA")" ;; esac
HERE="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$HERE/../../.." && pwd)"
# The SHA-256 verification gate resolves manifest paths against the current
# directory, so run every program from the repository root (same convention as
# the Makefile's test-sleela target). Default the manifest/sheet if unset.
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$REPO_ROOT/security/sha256-manifest.json}"
export SLEELA_SHEET="${SLEELA_SHEET:-$REPO_ROOT/SHEET.sheet}"
cd "$REPO_ROOT"
fail=0

run_one() {  # run_one <file>
    local file="$1"
    local label
    label="$(basename "$file")"
    local out
    if ! out="$("$SLEELA" run "$file" 2>/dev/null)"; then
        echo "  FAIL ($label did not run cleanly)"
        fail=1
        return
    fi
    # Only consider the PASS/FAIL assertion lines the program itself emits.
    local passes fails expected
    passes="$(printf '%s\n' "$out" | grep -c '^PASS ')"
    fails="$(printf '%s\n' "$out" | grep -c '^FAIL ')"
    expected="$(grep -oE 'EXPECTED_PASS=[0-9]+' "$file" | head -1 | cut -d= -f2)"

    if [ "$fails" -ne 0 ]; then
        echo "  FAIL ($label: $fails failing assertion(s))"
        printf '%s\n' "$out" | grep '^FAIL ' | sed 's/^/    > /'
        fail=1
        return
    fi
    if [ -n "$expected" ] && [ "$passes" -ne "$expected" ]; then
        echo "  FAIL ($label: expected $expected passing checks, got $passes)"
        fail=1
        return
    fi
    echo "  ok   ($label: $passes checks passed)"
}

echo "=== subject-library numeric assertions ==="
for f in \
    "$HERE/math_values.sleela" \
    "$HERE/physics_values.sleela" \
    "$HERE/economics_values.sleela" \
    "$HERE/inference_values.sleela" \
    "$HERE/financial_values.sleela" \
    "$HERE/chemistry_values.sleela"; do
    run_one "$f"
done

if [ "$fail" -eq 0 ]; then
    echo "SUBJECT TESTS: PASS"
    exit 0
else
    echo "SUBJECT TESTS: FAIL"
    exit 1
fi
