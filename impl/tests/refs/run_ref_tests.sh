#!/usr/bin/env bash
# =============================================================================
# run_ref_tests.sh -- assert the object-reference surface (docs/reference/POINTERS.md).
#
#   target.ref            the implicit VM-level reference of an object
#   Ptr.valid/id/gen/...  tier-2 views of a reference
#   Class.deref(p)        the typed way back to an object
#
# Covers identity, the tier-2 views, null/forged refs failing closed, a stale
# ref after collection, slot reuse never aliasing, the "declared field named
# ref wins" rule, read-only enforcement, and argument checking.
#
# Usage: run_ref_tests.sh [path-to-sleela-binary]   (run from anywhere)
# =============================================================================
set -u
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
BIN="${1:-$ROOT/impl/build/sleela}"
HERE="$ROOT/impl/tests/refs"
cd "$ROOT"
export SLEELA_SHEET="${SLEELA_SHEET:-$ROOT/SHEET.sheet}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"
fail=0

# Drop the visible security/memory records; they are not program output.
strip() { sed -E -e '/^verification: PASS \([0-9]+ item\(s\)\)$/d' \
                 -e '/^\[security\]/d' -e '/^\[defender\]/d' -e '/^\[memory-manager\]/d'; }

# expect_output <fixture> <label> <expected-text>
expect_output() {
    actual="$("$BIN" run "$HERE/$1" 2>&1 | strip)"
    if [ "$actual" != "$3" ]; then
        echo "  FAIL $2"; fail=1
        diff <(printf '%s\n' "$3") <(printf '%s\n' "$actual") | head -12
        return
    fi
    echo "  ok   $2"
}

# expect_reject <fixture> <label> <expected-error-substring>
expect_reject() {
    err="$("$BIN" run "$HERE/$1" 2>&1 >/dev/null)"
    if "$BIN" run "$HERE/$1" >/dev/null 2>&1; then
        echo "  FAIL (should have been rejected) $2"; fail=1; return
    fi
    if ! printf '%s' "$err" | grep -qF "$3"; then
        echo "  FAIL (wrong rejection; wanted '$3') $2"; fail=1; return
    fi
    echo "  ok   (rejected) $2"
}

basics=$'-- identity and the way back
true
suppose
true
true
false
-- tier 2 views of one object
1
true
true
true
true
true
false
4096
true
-- null and forged refs fail closed
0
false
-1
true
false
false
-- wrong class
true
-- a declared field named ref wins
7
-- weak: the ref outlives nothing
false
true
-1
true
true
-- slot reuse never aliases
false
true
fresh'

echo "object references (docs/reference/POINTERS.md)"
expect_output ref_basics.sleela "ref_basics: identity, tier-2 views, null/forged, stale, reuse" "$basics"
expect_output ref_user_class.sleela "a user class named Ptr takes over the name" "42"
expect_reject ref_assign_rejected.sleela "x.ref is read-only" "read-only"
expect_reject ref_bad_arg.sleela "Ptr.valid takes a ref (int)" "takes a ref (int)"
expect_reject ref_unknown_fn.sleela "Ptr has no such function" "Ptr has no function 'bogus'"

# The compiled artifact must validate and behave identically to the source run.
art="$(mktemp -t refbasics.XXXXXX)"
trap 'rm -f "$art"' EXIT
if "$BIN" compile "$HERE/ref_basics.sleela" -o "$art" >/dev/null 2>&1 \
   && "$BIN" validate-artifact "$art" 2>&1 | grep -q "artifact ABI ok"; then
    aout="$("$BIN" run "$art" 2>&1 | strip)"
    if [ "$aout" = "$basics" ]; then echo "  ok   artifact validates and runs identically"
    else echo "  FAIL artifact output differs from source run"; fail=1; fi
else
    echo "  FAIL artifact did not compile/validate"; fail=1
fi

if [ "$fail" -ne 0 ]; then echo "object references: FAIL"; exit 1; fi
echo "object references: PASS"
